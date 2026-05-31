//header file
#pragma once
#include "Mutator_base.h"

/**
 * Expand_Constexpr_Nested_Array_Initialization_476
 */ 
class MutatorFrontendAction_476 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(476)

private:
    class MutatorASTConsumer_476 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_476(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        const clang::VarDecl* targetVarDecl;
        const clang::Type* elementType;
        std::vector<unsigned> arrayDimensions;
        std::string newInitText;
        bool isConstexprContext;
    };
};

//source file
#include "../include/Mutator_Expand_Constexpr_Nested_Array_Initialization_476.h"

// ========================================================================================================
#define MUT476_OUTPUT 1

void MutatorFrontendAction_476::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("targetVarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      // Check if it's a constexpr context
      isConstexprContext = VD->isConstexpr() || VD->getType().isConstQualified();
      if (!isConstexprContext) {
        // Also check if inside a constexpr function
        const clang::DeclContext* DC = VD->getDeclContext();
        if (DC && DC->isFunctionOrMethod()) {
          const clang::FunctionDecl* FD = clang::dyn_cast<clang::FunctionDecl>(DC);
          if (FD && FD->isConstexpr()) {
            isConstexprContext = true;
          }
        }
      }
      
      if (!isConstexprContext) return;
      
      // Check if it's an array type
      const clang::Type* type = VD->getType().getTypePtrOrNull();
      if (!type) return;
      
      // Unwrap array types to get dimensions and element type
      elementType = type;
      arrayDimensions.clear();
      while (true) {
        if (const clang::ArrayType* AT = clang::dyn_cast<clang::ArrayType>(elementType)) {
          if (const clang::ConstantArrayType* CAT = clang::dyn_cast<clang::ConstantArrayType>(AT)) {
            arrayDimensions.push_back(CAT->getSize().getZExtValue());
            elementType = CAT->getElementType().getTypePtrOrNull();
          } else {
            // Not a constant array, skip
            return;
          }
        } else {
          break;
        }
      }
      
      // Need at least 2 dimensions for nested array
      if (arrayDimensions.size() < 2) return;
      
      // Check if element type is an aggregate struct/class
      const clang::RecordType* RT = clang::dyn_cast<clang::RecordType>(elementType);
      if (!RT) return;
      const clang::RecordDecl* RD = RT->getDecl();
      if (!RD || RD->hasUserDeclaredConstructor() || RD->hasUserDeclaredDefaultConstructor()) return;
      
      targetVarDecl = VD;
      
      // Get the initializer
      const clang::Expr* init = VD->getInit();
      if (!init) return;
      
      // Get source text of initializer
      std::string initText = stringutils::rangetoStr(*(Result.SourceManager), init->getSourceRange());
      
      // Analyze initializer list structure
      // We need to expand nested braces
      std::string newInit = "{";
      
      // For M x N array, we need M rows each with N elements
      unsigned M = arrayDimensions[0];
      unsigned N = arrayDimensions[1];
      
      // Parse the existing initializer list
      // Simple heuristic: count commas at top level to estimate rows provided
      size_t topLevelCommas = 0;
      size_t depth = 0;
      for (size_t i = 0; i < initText.length(); ++i) {
        if (initText[i] == '{') depth++;
        else if (initText[i] == '}') depth--;
        else if (initText[i] == ',' && depth == 1) topLevelCommas++;
      }
      
      unsigned rowsProvided = topLevelCommas + 1;
      
      // Build new initializer with explicit nested braces
      for (unsigned i = 0; i < M; ++i) {
        newInit += "{";
        if (i < rowsProvided) {
          // Try to extract this row's initializers from original text
          // This is simplified - actual parsing would be more complex
          // For now, we'll just use placeholder
          newInit += "/* row " + std::to_string(i) + " */";
        }
        // Add N elements per row
        for (unsigned j = 0; j < N; ++j) {
          if (j > 0) newInit += ", ";
          newInit += "{}"; // value-initialized element
        }
        newInit += "}";
        if (i < M-1) newInit += ", ";
      }
      newInit += "}";
      
      newInitText = newInit;
      
      // Perform mutation on the source code text by applying string replacement
      std::string replacement = VD->getNameAsString() + " = " + newInitText + "; (void)(" + RD->getNameAsString() + "{});";
      
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), replacement);
    }
}
  
void MutatorFrontendAction_476::MutatorASTConsumer_476::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer(initListExpr()), unless(isImplicit())).bind("targetVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
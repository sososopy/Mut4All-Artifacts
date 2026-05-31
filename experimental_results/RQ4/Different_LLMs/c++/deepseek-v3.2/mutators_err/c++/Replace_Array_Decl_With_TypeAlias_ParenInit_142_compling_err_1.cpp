//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Array_Decl_With_TypeAlias_ParenInit_142
 */ 
class MutatorFrontendAction_142 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(142)

private:
    class MutatorASTConsumer_142 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_142(Rewriter &R) : TheRewriter(R) {}
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
        std::map<std::string, std::string> existingAliases;
    };
};

//source file
#include "../include/Mutator_Replace_Array_Decl_With_TypeAlias_ParenInit_142.h"

// ========================================================================================================
#define MUT142_OUTPUT 1

void MutatorFrontendAction_142::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayVarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      // Check if variable has array type
      const clang::Type *type = VD->getType().getTypePtr();
      if (!type->isArrayType() && !type->isConstantArrayType() && !type->isIncompleteArrayType())
        return;

      // Get the array type as string
      clang::QualType arrayQualType = VD->getType();
      std::string arrayTypeStr = arrayQualType.getAsString();
      
      // Check for existing type alias
      std::string aliasName;
      if (existingAliases.find(arrayTypeStr) != existingAliases.end()) {
        aliasName = existingAliases[arrayTypeStr];
      } else {
        // Generate new alias name
        static int aliasCounter = 0;
        aliasName = "ArrayAlias_" + std::to_string(aliasCounter++);
        existingAliases[arrayTypeStr] = aliasName;
        
        // Insert type alias before the variable declaration
        std::string aliasDecl = "using " + aliasName + " = " + arrayTypeStr + ";\n";
        Rewrite.InsertTextBefore(VD->getBeginLoc(), "/*mut142*/" + aliasDecl);
      }

      // Get initializers if present
      std::string initStr = "";
      if (VD->hasInit()) {
        clang::Expr *initExpr = VD->getInit();
        if (auto *initList = clang::dyn_cast<clang::InitListExpr>(initExpr)) {
          // Convert brace initialization to parentheses
          for (unsigned i = 0; i < initList->getNumInits(); ++i) {
            if (i > 0) initStr += ", ";
            clang::Expr *init = initList->getInit(i);
            initStr += stringutils::rangetoStr(*(Result.SourceManager), init->getSourceRange());
          }
        }
      }

      // Construct new variable declaration with parentheses initialization
      std::string newDecl = aliasName + " " + VD->getNameAsString();
      if (!initStr.empty()) {
        newDecl += "(" + initStr + ")";
      }
      newDecl += ";";

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), "/*mut142*/" + newDecl);
    }
}
  
void MutatorFrontendAction_142::MutatorASTConsumer_142::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(arrayType())).bind("arrayVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
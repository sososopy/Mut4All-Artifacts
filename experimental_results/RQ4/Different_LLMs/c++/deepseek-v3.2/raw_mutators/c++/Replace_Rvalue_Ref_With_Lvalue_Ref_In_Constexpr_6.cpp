//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Rvalue_Ref_With_Lvalue_Ref_In_Constexpr_6
 */ 
class MutatorFrontendAction_6 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(6)

private:
    class MutatorASTConsumer_6 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_6(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> targetVarDecls;
    };
};

//source file
#include "../include/Replace_Rvalue_Ref_With_Lvalue_Ref_In_Constexpr_6.h"

// ========================================================================================================
#define MUT6_OUTPUT 1

void MutatorFrontendAction_6::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("rvalueRefVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      // Check if it's an rvalue reference
      if (!VD->getType()->isRValueReferenceType())
        return;
      
      // Check if this variable is used inside a constexpr member function of a local struct
      bool usedInConstexprLocalStruct = false;
      const auto* DC = VD->getDeclContext();
      if (!DC || !DC->isFunctionOrMethod())
        return;
      
      // Record the variable for later processing
      targetVarDecls.push_back(VD);
    }
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("localStruct")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      
      // Check if it's a local struct (defined inside a function)
      const auto* DC = RD->getDeclContext();
      if (!DC || !DC->isFunctionOrMethod())
        return;
      
      // Check if it has any constexpr member functions
      bool hasConstexprMethod = false;
      for (const auto* D : RD->decls()) {
        if (const auto* MD = dyn_cast<clang::CXXMethodDecl>(D)) {
          if (MD->isConstexpr()) {
            hasConstexprMethod = true;
            break;
          }
        }
      }
      
      if (!hasConstexprMethod)
        return;
      
      // Now process all recorded rvalue reference variables
      for (const auto* VD : targetVarDecls) {
        // Check if this variable is in the same function scope as the local struct
        const auto* varDC = VD->getDeclContext();
        if (varDC != DC)
          continue;
        
        // Get the variable name
        std::string varName = VD->getNameAsString();
        if (varName.empty())
          continue;
        
        // Check if this variable is used inside the local struct's constexpr methods
        bool varUsedInStruct = false;
        for (const auto* D : RD->decls()) {
          if (const auto* MD = dyn_cast<clang::CXXMethodDecl>(D)) {
            if (MD->isConstexpr()) {
              // Simple check: see if variable name appears in the method's source range
              SourceRange methodRange = MD->getSourceRange();
              SourceManager& SM = Rewrite.getSourceMgr();
              const char* methodStart = SM.getCharacterData(methodRange.getBegin());
              const char* methodEnd = SM.getCharacterData(methodRange.getEnd());
              std::string methodText(methodStart, methodEnd - methodStart);
              
              if (methodText.find(varName) != std::string::npos) {
                varUsedInStruct = true;
                break;
              }
            }
          }
        }
        
        if (!varUsedInStruct)
          continue;
        
        // Get the source code text of target node
        std::string varDeclText = stringutils::rangetoStr(*(Result.SourceManager), VD->getSourceRange());
        
        // Get the type without reference
        QualType baseType = VD->getType()->getPointeeType();
        if (baseType.isNull())
          continue;
        
        // Create new variable name
        std::string tempVarName = "temp_" + varName;
        
        // Perform mutation: replace rvalue reference with lvalue reference
        // First, insert a new non-reference variable before the original declaration
        std::string newCode = "/*mut6*/" + baseType.getAsString() + " " + tempVarName + " = ";
        
        // Get the initializer
        if (VD->hasInit()) {
          std::string initText = stringutils::rangetoStr(*(Result.SourceManager), 
                                                         VD->getInit()->getSourceRange());
          newCode += initText + ";\n";
        } else {
          newCode += "{};\n";
        }
        
        // Then replace the original declaration with lvalue reference
        newCode += baseType.getAsString() + "& " + varName + " = " + tempVarName + ";";
        
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), newCode);
        
        // Only mutate one variable per local struct to avoid conflicts
        break;
      }
      
      // Clear the recorded variables for next local struct
      targetVarDecls.clear();
    }
}
  
void MutatorFrontendAction_6::MutatorASTConsumer_6::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    // First matcher for rvalue reference variables
    DeclarationMatcher varMatcher = varDecl(hasType(rValueReferenceType())).bind("rvalueRefVar");
    // Second matcher for local structs
    DeclarationMatcher structMatcher = cxxRecordDecl(isDefinition(), unless(isExpansionInSystemHeader())).bind("localStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(structMatcher, &callback);
    matchFinder.matchAST(Context);
}
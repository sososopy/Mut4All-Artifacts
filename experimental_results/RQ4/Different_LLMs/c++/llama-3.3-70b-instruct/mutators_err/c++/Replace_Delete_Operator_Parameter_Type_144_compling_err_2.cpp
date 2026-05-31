//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Delete_Operator_Parameter_Type_144
 */ 
class MutatorFrontendAction_144 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(144)

private:
    class MutatorASTConsumer_144 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_144(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Replace_Delete_Operator_Parameter_Type_144.h"

// ========================================================================================================
#define MUT144_OUTPUT 1

void MutatorFrontendAction_144::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("DeleteOperator")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto param = MT->getParamDecl(0);
      if (!param)
        return;
      if (param->getType()->isPointerType() && param->getType()->getPointeeType()->isClassType()) {
        //Get all class types in the translation unit
        std::vector<const clang::CXXRecordDecl*> classes;
        for (auto &D : Result.Context->getTranslationUnitDecl()->decls()) {
          if (auto *C = dyn_cast<clang::CXXRecordDecl>(&D)) {
            classes.push_back(C);
          }
        }
        if (classes.size() > 1) {
          //Replace the original type with another existing type
          auto newType = classes[1]->getTypeForDecl();
          auto newParamStr = newType->getAsString();
          Rewrite.ReplaceText(param->getBeginLoc(), param->getType()->getAsString());
        } else {
          //Introduce a new type if no other class types exist
          std::string newTypeStr = "struct NewType;";
          Rewrite.InsertTextAfterToken(MT->getBeginLoc(), newTypeStr);
          auto newParamStr = "NewType*";
          Rewrite.ReplaceText(param->getBeginLoc(), param->getType()->getAsString());
        }
      }
    }
}
  
void MutatorFrontendAction_144::MutatorASTConsumer_144::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(hasOverloadedOperatorName("delete")).bind("DeleteOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
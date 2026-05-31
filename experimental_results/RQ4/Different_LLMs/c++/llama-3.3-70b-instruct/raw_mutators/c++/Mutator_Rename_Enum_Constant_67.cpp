//header file
#pragma once
#include "Mutator_base.h"

/**
 * Rename_Enum_Constant_67
 */ 
class MutatorFrontendAction_67 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(67)

private:
    class MutatorASTConsumer_67 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_67(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_Rename_Enum_Constant_67.h"

// ========================================================================================================
#define MUT67_OUTPUT 1

void MutatorFrontendAction_67::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Enum = Result.Nodes.getNodeAs<clang::EnumConstantDecl>("EnumConstant")) {
      //Filter nodes in header files
      if (!Enum || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Enum->getLocation()))
        return;
      //Get the source code text of target node
      std::string newName = "NEW_" + Enum->getNameAsString();
      //Perform mutation on the source code text by applying string replacement
      Rewrite.ReplaceText(Enum->getLocation(), Enum->getNameAsString(), newName);
    }
}
  
void MutatorFrontendAction_67::MutatorASTConsumer_67::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumConstantDecl().bind("EnumConstant");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
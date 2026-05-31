//header file
#pragma once
#include "Mutator_base.h"

/**
 * Convert_Enum_To_Forward_Declaration_2
 */ 
class MutatorFrontendAction_2 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(2)

private:
    class MutatorASTConsumer_2 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_2(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Convert_Enum_To_Forward_Declaration_2.h"

// ========================================================================================================
#define MUT2_OUTPUT 1

void MutatorFrontendAction_2::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             ED->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (content.find("struct") != string::npos)
        content.replace(content.find("struct"), 6, "");
      else if (content.find("class") != string::npos)
        content.replace(content.find("class"), 5, "");
      if (content.find('{') != string::npos)
        content.erase(content.find('{'));
      if (content.rfind('}') != string::npos)
        content.erase(content.rfind('}'));
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_2::MutatorASTConsumer_2::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
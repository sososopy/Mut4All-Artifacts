//header file
#pragma once
#include "Mutator_base.h"

/**
 * Access_NonStatic_Member_Function_Via_Class_37
 */ 
class MutatorFrontendAction_37 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(37)

private:
    class MutatorASTConsumer_37 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_37(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Access_NonStatic_Member_Function_Via_Class_37.h"

// ========================================================================================================
#define MUT37_OUTPUT 1

void MutatorFrontendAction_37::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::DeclRefExpr>("DeclRef")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      //Get the source code text of target node
      auto name = DL->getNameInfo().getAsString();
      if (name != "is_invocable_v")
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          DL->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      text = "/*mut37*/" + text;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(DL->getBeginLoc(), 0, text);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::DecltypeType>(
                   "DecltypeType")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          DL->getSourceRange());
      if (text.find("::") == string::npos)
        return;
      //Perform mutation on the source code text by applying string replacement
      text = "/*mut37*/" + text;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(DL->getBeginLoc(), 0, text);
    }
}
  
void MutatorFrontendAction_37::MutatorASTConsumer_37::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = declRefExpr().bind("DeclRef");
    auto decltype_matcher = decltypeType().bind("DecltypeType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(decltype_matcher, &callback);
    matchFinder.matchAST(Context);
}
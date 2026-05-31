//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Requires_Expression_In_Function_Body_355
 */ 
class MutatorFrontendAction_355 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(355)

private:
    class MutatorASTConsumer_355 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_355(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> candidate_functions;
    };
};

//source file
#include "../include/insert_requires_expression_in_function_body_355.h"

// ========================================================================================================
#define MUT355_OUTPUT 1

void MutatorFrontendAction_355::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->hasBody())
        candidate_functions.push_back(FD);
    } else if (auto *MD =
                   Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;
      if (MD->hasBody() && !MD->isVirtual())
        candidate_functions.push_back(MD);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                   "CandidateFunction")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      auto body = FD->getBody();
      auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              body->getSourceRange());
      body_str.insert(body_str.rfind('}'), "\n/*mut355*/requires { requires (int; }");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()),
                          body_str);
    }
}
  
void MutatorFrontendAction_355::MutatorASTConsumer_355::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto method_matcher =
        cxxMethodDecl(unless(cxxConstructorDecl())).bind("Methods");
    auto candidate_matcher = functionDecl().bind("CandidateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(method_matcher, &callback);
    matchFinder.matchAST(Context);
    if (candidate_functions.empty())
      return;
    size_t index = getrandom::getRandomIndex(candidate_functions.size() - 1);
    auto target = candidate_functions[index];
    llvm::outs() << "Mut355: " << target->getNameAsString() << '\n';
    MatchFinder finder2;
    finder2.addMatcher(candidate_matcher, &callback);
    finder2.matchAST(Context);
}
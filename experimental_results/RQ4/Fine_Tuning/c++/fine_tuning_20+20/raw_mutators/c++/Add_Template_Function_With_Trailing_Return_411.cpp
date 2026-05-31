//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Template_Function_With_Trailing_Return_411
 */ 
class MutatorFrontendAction_411 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(411)

private:
    class MutatorASTConsumer_411 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_411(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/add_template_function_with_trailing_return_411.h"

// ========================================================================================================
#define MUT411_OUTPUT 1

void MutatorFrontendAction_411::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    if (content.find("template") != string::npos) {
      cur_classes.push_back(DL);
    }
  } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Main")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    if (FD->isMain() == false)
      return;
    if (cur_classes.empty())
      return;
    auto target = cur_classes[getrandom::getRandomIndex(cur_classes.size() - 1)];
    auto target_name = target->getNameAsString();
    llvm::outs() << target_name << '\n';
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
    if (content.rfind('}') != string::npos) {
      content.insert(content.rfind('}'),
                     "\n/*mut411*/\n" + target_name + "<int>().f(A<int>(), D());\n");
    }
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()),
                        content);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("A")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    if (DL->getNameAsString() != "A")
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    auto ins = "\n/*mut411*/template <class U, class V> auto f(U u, V v) -> decltype(u.template B<V>::MEM) { return u.template B<V>::MEM; }\n";
    if (content.rfind('}') != string::npos) {
      content.insert(content.rfind('}'), ins);
    }
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                        content);
  }
}
  
void MutatorFrontendAction_411::MutatorASTConsumer_411::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
  auto main_matcher = functionDecl().bind("Main");
  auto a_matcher = cxxRecordDecl().bind("A");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(main_matcher, &callback);
  matchFinder.addMatcher(a_matcher, &callback);
  matchFinder.matchAST(Context);
}
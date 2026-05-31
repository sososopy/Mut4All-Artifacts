//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inject_Templated_Friend_Function_With_Static_Assertions_179
 */ 
class MutatorFrontendAction_179 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(179)

private:
    class MutatorASTConsumer_179 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_179(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::CXXRecordDecl *> cur_classes;
        std::vector<string> cur_funcnames;
    };
};

//source file
#include "../include/inject_templated_friend_function_with_static_assertions_179.h"

// ========================================================================================================
#define MUT179_OUTPUT 1

void MutatorFrontendAction_179::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    llvm::outs() << content << '\n';
    auto funcname = "fn";
    if (content.find(funcname) != string::npos) {
      cur_classes.push_back(DL);
      cur_funcnames.push_back(funcname);
    }
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(

                 "ClassesWithFn")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    llvm::outs() << content << '\n';
    auto funcname = "fn";
    if (content.find(funcname) != string::npos) {
      cur_classes.push_back(DL);
      cur_funcnames.push_back(funcname);
    }
  } else if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                 "ClassesWithFn")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    if (CL->isCompleteDefinition() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), CL->getSourceRange());
    llvm::outs() << content << '\n';
    if (content.find("fn") == string::npos)
      return;
    auto ins = R"(
template <typename... Args> friend void fn() {
  static_assert(sizeof...(Args) != 0);
  static_assert(sizeof...(Args) == 0);
  constexpr auto v = sizeof...(Args);
  fn<v>();
}
)";
    ins = string("/*mut179*/") + ins;
    Rewrite.ReplaceText(CL->getEndLoc(), 0, ins);
  } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>(

                 "FnDecl")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
    llvm::outs() << content << '\n';
    if (FD->getNameAsString() != "fn")
      return;
    if (FD->isTemplateInstantiation()) {
      return;
    }
    if (FD->getNumParams() != 1)
      return;
    auto param = FD->getParamDecl(0);
    if (param->getType().getAsString() != "int")
      return;
    auto ins = R"(
fn<int, char>();
fn<>();
)";
    ins = string("/*mut179*/") + ins;
    Rewrite.ReplaceText(FD->getEndLoc(), 0, ins);
  }
}
  
void MutatorFrontendAction_179::MutatorASTConsumer_179::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
  auto matcher2 = cxxRecordDecl(has(cxxMethodDecl(hasName("fn"))))
                      .bind("ClassesWithFn");
  auto matcher3 = functionDecl(hasName("fn")).bind("FnDecl");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(matcher2, &callback);
  matchFinder.addMatcher(matcher3, &callback);
  matchFinder.matchAST(Context);
}
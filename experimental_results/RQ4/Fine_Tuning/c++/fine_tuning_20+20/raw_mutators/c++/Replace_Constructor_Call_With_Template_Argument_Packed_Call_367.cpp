//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constructor_Call_With_Template_Argument_Packed_Call_367
 */ 
class MutatorFrontendAction_367 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(367)

private:
    class MutatorASTConsumer_367 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_367(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Constructor_Call_With_Template_Argument_Packed_Call_367.h"

// ========================================================================================================
#define MUT367_OUTPUT 1

void MutatorFrontendAction_367::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    if (FD->isDefaulted() || FD->isDeleted())
      return;
    if (!FD->hasBody())
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
    llvm::outs() << content << '\n';
    for (auto cur_class : cur_classes) {
      auto classname = cur_class->getNameAsString();
      llvm::outs() << classname << '\n';
      auto pos = content.find(classname + '(');
      if (pos != string::npos) {
        content.replace(pos, classname.size(), classname + "::createWithArgs");
      }
    }
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()),
                        content);
  } else if (auto *CL =
                 Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    if (!CL->isCompleteDefinition())
      return;
    if (CL->isLambda())
      return;
    cur_classes.push_back(CL);
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), CL->getSourceRange());
    if (content.rfind('}') != string::npos) {
      content.insert(content.rfind('}'),
                     "\n/*mut367*/template<typename... Args> static " +
                         CL->getNameAsString() +
                         " createWithArgs(Args&&... args) { return " +
                         CL->getNameAsString() +
                         "(std::forward<Args>(args)...); }\n");
    }
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(CL->getSourceRange()),
                        content);
  }
}
  
void MutatorFrontendAction_367::MutatorASTConsumer_367::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
  auto class_matcher = cxxRecordDecl().bind("Classes");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(class_matcher, &callback);
  matchFinder.matchAST(Context);
}
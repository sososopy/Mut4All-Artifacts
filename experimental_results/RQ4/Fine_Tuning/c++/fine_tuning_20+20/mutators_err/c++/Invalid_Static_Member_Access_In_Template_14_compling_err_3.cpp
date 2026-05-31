//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Static_Member_Access_In_Template_14
 */ 
class MutatorFrontendAction_14 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(14)

private:
    class MutatorASTConsumer_14 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_14(Rewriter &R) : TheRewriter(R) {}
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
        std::string cur_class_name;
        std::string cur_func_name;
    };
};

//source file
#include "../include/mutator_14.h"

// ========================================================================================================
#define MUT14_OUTPUT 1

void MutatorFrontendAction_14::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isCompleteDefinition())
        return;
      if (!DL->isTemplated())
        return;
      cur_class_name = DL->getNameAsString();
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isStatic())
        return;
      cur_func_name = FD->getNameAsString();
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("Calls")) {
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      if (cur_class_name == "" || cur_func_name == "")
        return;
      auto callee = CE->getCalleeDecl();
      if (!callee)
        return;
      auto callee_name = callee->getNameAsString();
      llvm::outs() << callee_name << '\n';
      if (callee_name != cur_func_name)
        return;
      auto call_text = stringutils::rangetoStr(*(Result.SourceManager),
                                               CE->getSourceRange());
      if (call_text.find("this->") != string::npos)
        return;
      if (call_text.find("this") != string::npos) {
        call_text = stringutils::replace_first(call_text, cur_class_name,
                                               "this");
      } else {
        call_text =
            stringutils::replace_first(call_text, cur_class_name, "this->");
      }
      llvm::outs() << call_text << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()),
                          call_text);
    }
}
  
void MutatorFrontendAction_14::MutatorASTConsumer_14::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto func_matcher = functionDecl().bind("Functions");
    auto call_matcher = callExpr().bind("Calls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.addMatcher(call_matcher, &callback);
    matchFinder.matchAST(Context);
}
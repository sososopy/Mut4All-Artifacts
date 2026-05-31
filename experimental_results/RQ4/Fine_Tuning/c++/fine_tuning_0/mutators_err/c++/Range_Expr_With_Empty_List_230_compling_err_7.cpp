//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_For-Range_Expr_With_Empty_List_230
 */ 
class MutatorFrontendAction_230 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(230)

private:
    class MutatorASTConsumer_230 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_230(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/Replace_For-Range_Expr_With_Empty_List_230.h"

// ========================================================================================================
#define MUT230_OUTPUT 1

void MutatorFrontendAction_230::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    cur_classes.push_back(DL);
  } else if (auto *FS = Result.Nodes.getNodeAs<clang::CXXForRangeStmt>(
                 "ForRange")) {
    if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FS->getBeginLoc()))
      return;

    if (cur_classes.empty())
      return;
    auto range = FS->getRangeInit();
    auto range_str = stringutils::rangetoStr(*(Result.SourceManager),
                                             range->getSourceRange());
    llvm::outs() << range_str << "\n";
    size_t index = getrandom::getRandomIndex(cur_classes.size() - 1);
    auto target = cur_classes[index];
    auto ins = target->getNameAsString() + "{}";
    llvm::outs() << ins << "\n";
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(range->getSourceRange()),
                        ins);
  }
}
  
void MutatorFrontendAction_230::MutatorASTConsumer_230::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
  auto forrange_matcher = forRangeStmt().bind("ForRange");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(forrange_matcher, &callback);
  matchFinder.matchAST(Context);
}
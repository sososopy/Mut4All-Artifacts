//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enclose_Function_And_Global_Statements_With_Ifndef_Preamble_Else_396
 */ 
class MutatorFrontendAction_396 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(396)

private:
    class MutatorASTConsumer_396 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_396(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::Stmt *> global_stmts;
    };
};

//source file
#include "../include/enclose_function_and_global_statements_with_ifndef_preamble_else_396.h"

// ========================================================================================================
#define MUT396_OUTPUT 1

void MutatorFrontendAction_396::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getBeginLoc()))
        return;
      if (!FD->hasBody())
        return;
      auto body = FD->getBody();
      auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              body->getSourceRange());
      if (!global_stmts.empty()) {
        auto choice = getrandom::getRandomIndex(global_stmts.size() - 1);
        auto ins_stmt = global_stmts[choice];
        auto ins_str = stringutils::rangetoStr(*(Result.SourceManager),
                                               ins_stmt->getSourceRange());
        body_str = "/*mut396*/#ifndef PREAMBLE\n" + body_str + "\n#else\n" +
                   ins_str + "\n#endif\n";
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(body->getSourceRange()), body_str);
      }
    } else if (auto *GS =
                   Result.Nodes.getNodeAs<clang::Stmt>("GlobalStatements")) {
      if (!GS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     GS->getBeginLoc()))
        return;
      global_stmts.push_back(GS);
      auto gs_str = stringutils::rangetoStr(*(Result.SourceManager),
                                            GS->getSourceRange());
      gs_str = "/*mut396*/#ifndef PREAMBLE\n" + gs_str + "\n#endif\n";
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(GS->getSourceRange()), gs_str);
    }
}

void MutatorFrontendAction_396::MutatorASTConsumer_396::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto func_matcher = functionDecl().bind("Functions");
    auto global_stmt_matcher = stmt(anyOf(declStmt(), exprStmt())).bind(
        "GlobalStatements");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.addMatcher(global_stmt_matcher, &callback);
    matchFinder.matchAST(Context);
}
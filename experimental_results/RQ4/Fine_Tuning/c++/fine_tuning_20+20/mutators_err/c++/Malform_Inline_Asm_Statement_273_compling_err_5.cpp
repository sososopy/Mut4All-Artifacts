//header file
#pragma once
#include "Mutator_base.h"

/**
 * Malform_Inline_Asm_Statement_273
 */ 
class MutatorFrontendAction_273 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(273)

private:
    class MutatorASTConsumer_273 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_273(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> cur_functions;
    };
};

//source file
#include "../include/Malform_Inline_Asm_Statement_273.h"

// ========================================================================================================
#define MUT273_OUTPUT 1

void MutatorFrontendAction_273::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AS = Result.Nodes.getNodeAs<clang::GCCAsmStmt>("Asm")) {
      if (!AS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AS->getBeginLoc()))
        return;
      auto asmtext = stringutils::rangetoStr(*(Result.SourceManager),
                                             AS->getSourceRange());
      auto asm_begin = asmtext.find("asm");
      if (asm_begin == string::npos)
        return;
      asmtext.insert(asm_begin, "/*mut273*/");
      if (AS->isVolatile()) {
        auto vol_begin = asmtext.find("volatile");
        if (vol_begin != string::npos)
          asmtext.erase(vol_begin, 8);
      }
      if (AS->getNumOutputs() > 0) {
        auto colon_begin = asmtext.find(':');
        if (colon_begin != string::npos)
          asmtext.erase(colon_begin, 1);
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(AS->getSourceRange()),
                          asmtext);
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getBeginLoc()))
        return;
      if (FD->hasBody())
        cur_functions.push_back(FD);
    } else if (auto *CS = Result.Nodes.getNodeAs<clang::CompoundStmt>(
                   "CompoundStmts")) {
      if (!CS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CS->getBeginLoc()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             CS->getSourceRange());
      if (content.find("asm") != string::npos)
        return;
      int choice = getrandom::getRandomIndex(1);
      if (choice) {
        content.insert(content.find('{') + 1, "\n/*mut273*/asm(\"mut273\");");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(CS->getSourceRange()), content);
      }
    }
}

void MutatorFrontendAction_273::MutatorASTConsumer_273::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto asm_matcher = clang::ast_matchers::gccAsmStmt().bind("Asm");
    auto function_matcher = clang::ast_matchers::functionDecl().bind("Functions");
    auto compound_matcher = clang::ast_matchers::compoundStmt().bind("CompoundStmts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(asm_matcher, &callback);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.addMatcher(compound_matcher, &callback);
    matchFinder.matchAST(Context);
}
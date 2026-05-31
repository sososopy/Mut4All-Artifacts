//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Preprocessor_Directive_227
 */ 
class MutatorFrontendAction_227 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(227)

private:
    class MutatorASTConsumer_227 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_227(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/insert_invalid_preprocessor_directive_227.h"

// ========================================================================================================
#define MUT227_OUTPUT 1

void MutatorFrontendAction_227::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *InitList = Result.Nodes.getNodeAs<clang::InitListExpr>("initList")) {
      //Filter nodes in header files
      if (!InitList || !Result.Context->getSourceManager().isWrittenInMainFile(
                     InitList->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto sourceRange = InitList->getSourceRange();
      auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      
      //Perform mutation on the source code text by applying string replacement
      std::string invalidDirective = "\n#embed __FILE__ nonsense(10) limit(2)\n";
      size_t insertPos = sourceText.find("{") + 1;
      sourceText.insert(insertPos, invalidDirective);

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceText);
    }
}

void MutatorFrontendAction_227::MutatorASTConsumer_227::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = initListExpr().bind("initList");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
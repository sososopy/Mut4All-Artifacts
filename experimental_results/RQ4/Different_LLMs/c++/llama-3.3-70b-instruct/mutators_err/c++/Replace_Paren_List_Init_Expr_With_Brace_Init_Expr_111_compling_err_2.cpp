//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Paren_List_Init_Expr_With_Brace_Init_Expr_111
 */ 
class MutatorFrontendAction_Replace_Paren_List_Init_Expr_With_Brace_Init_Expr_111 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_Replace_Paren_List_Init_Expr_With_Brace_Init_Expr_111>(TheRewriter);
    }

private:
    class MutatorASTConsumer_Replace_Paren_List_Init_Expr_With_Brace_Init_Expr_111 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Replace_Paren_List_Init_Expr_With_Brace_Init_Expr_111(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Paren_List_Init_Expr_With_Brace_Init_Expr_111.h"

// ========================================================================================================
#define REPLACE_PAREN_LIST_INIT_EXPR_WITH_BRACE_INIT_EXPR_111_OUTPUT 1

void MutatorFrontendAction_Replace_Paren_List_Init_Expr_With_Brace_Init_Expr_111::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::InitListExpr>("InitListExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedDeclaration = declaration;
      size_t openParenPos = mutatedDeclaration.find('(');
      size_t closeParenPos = mutatedDeclaration.find(')');
      if (openParenPos != std::string::npos && closeParenPos != std::string::npos) {
        mutatedDeclaration.replace(openParenPos, 1, "{");
        mutatedDeclaration.replace(closeParenPos, 1, "}");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_Replace_Paren_List_Init_Expr_With_Brace_Init_Expr_111::MutatorASTConsumer_Replace_Paren_List_Init_Expr_With_Brace_Init_Expr_111::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = initListExpr().bind("InitListExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Constexpr_Expression_189
 */ 
class MutatorFrontendAction_189 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(189)

private:
    class MutatorASTConsumer_189 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_189(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutate_Constexpr_Expression_189.h"

// ========================================================================================================
#define MUT189_OUTPUT 1

void MutatorFrontendAction_189::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::Expr>("Constexpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto expr = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (expr.find('+') != string::npos)
        expr.replace(expr.find('+'), 1, "-");
      else if (expr.find('-') != string::npos)
        expr.replace(expr.find('-'), 1, "+");
      else if (expr.find('*') != string::npos)
        expr.replace(expr.find('*'), 1, "/");
      else if (expr.find('/') != string::npos)
        expr.replace(expr.find('/'), 1, "*");
      else if (expr.find('%') != string::npos)
        expr.replace(expr.find('%'), 1, "*");
      else if (expr.find('&') != string::npos)
        expr.replace(expr.find('&'), 1, "|");
      else if (expr.find('|') != string::npos)
        expr.replace(expr.find('|'), 1, "&");
      else if (expr.find('^') != string::npos)
        expr.replace(expr.find('^'), 1, "&");
      else if (expr.find('>') != string::npos)
        expr.replace(expr.find('>'), 1, "<");
      else if (expr.find('<') != string::npos)
        expr.replace(expr.find('<'), 1, ">");
      else if (expr.find('!') != string::npos)
        expr.replace(expr.find('!'), 1, "");
      else if (expr.find('~') != string::npos)
        expr.replace(expr.find('~'), 1, "");
      else if (expr.find("true") != string::npos)
        expr.replace(expr.find("true"), 4, "false");
      else if (expr.find("false") != string::npos)
        expr.replace(expr.find("false"), 5, "true");
      else if (expr.find("0") != string::npos)
        expr.replace(expr.find("0"), 1, "1");
      else if (expr.find("1") != string::npos)
        expr.replace(expr.find("1"), 1, "0");
      else
        return;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          expr);
    }
}
  
void MutatorFrontendAction_189::MutatorASTConsumer_189::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = expr(hasDescendant(constantExpr())).bind("Constexpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
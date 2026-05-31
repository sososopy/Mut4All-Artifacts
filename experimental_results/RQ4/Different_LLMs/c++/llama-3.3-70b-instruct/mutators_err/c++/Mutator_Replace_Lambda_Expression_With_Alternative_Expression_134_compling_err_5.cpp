//header file
#pragma once
#include "Mutator_base.h"
#include <regex>

/**
 * Replace_Lambda_Expression_With_Alternative_Expression_134
 */ 
class MutatorFrontendAction_134 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(134)

private:
    class MutatorASTConsumer_134 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_134(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Lambda_Expression_With_Alternative_Expression_134.h"
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

// ========================================================================================================
#define MUT134_OUTPUT 1

void MutatorFrontendAction_134::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RE = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("RequiresExpr")) {
      //Filter nodes in header files
      if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RE->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   RE->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Find the lambda expression in the requires expression
      std::regex lambdaRegex("\\[&\\]\\s*->\\s*decltype\\((.*?)\\)\\s*\\{\\s*\\}");
      std::smatch lambdaMatch;
      if (std::regex_search(declaration, lambdaMatch, lambdaRegex)) {
        // Replace the lambda expression with an alternative expression
        std::string alternativeExpression = lambdaMatch[1].str();
        declaration = std::regex_replace(declaration, lambdaRegex, alternativeExpression + ";");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RE->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_134::MutatorASTConsumer_134::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(has(cxxRequiresClause())).bind("RequiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Mismatched_Parentheses_In_Requires_Clause_116
 */ 
class MutatorFrontendAction_116 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(116)

private:
    class MutatorASTConsumer_116 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_116(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Introduce_Mismatched_Parentheses_In_Requires_Clause_116.h"

// ========================================================================================================
#define MUT116_OUTPUT 1

void MutatorFrontendAction_116::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (const auto *RE = Result.Nodes.getNodeAs<clang::RequiresExpr>("RequiresExpr")) {
      //Filter nodes in header files
      if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto sourceRange = RE->getSourceRange();
      auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      
      //Check if requires expression has parameters (parentheses)
      bool hasParams = false;
      SourceLocation insertLoc;
      std::string mutatedText;
      
      // We cannot directly access RequiresExpr methods due to incomplete type,
      // so we rely on source text analysis
      size_t lparenPos = sourceText.find('(');
      if (lparenPos != std::string::npos) {
        hasParams = true;
        //Find location after opening '(' of parameter list
        auto LParenLoc = sourceRange.getBegin().getLocWithOffset(lparenPos);
        if (LParenLoc.isValid()) {
          insertLoc = LParenLoc.getLocWithOffset(1);
          //Insert extra '(' after the opening '('
          mutatedText = sourceText;
          mutatedText.insert(lparenPos + 1, "(");
        }
      } else {
        //No parameters: change "requires {" to "requires ({"
        size_t requiresPos = sourceText.find("requires");
        size_t bodyPos = sourceText.find('{');
        if (requiresPos != std::string::npos && bodyPos != std::string::npos) {
          insertLoc = sourceRange.getBegin().getLocWithOffset(bodyPos);
          mutatedText = sourceText;
          mutatedText.insert(bodyPos, "(");
        }
      }
      
      if (!mutatedText.empty()) {
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), "/*mut116*/" + mutatedText);
      }
    }
}
  
void MutatorFrontendAction_116::MutatorASTConsumer_116::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = requiresExpr().bind("RequiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
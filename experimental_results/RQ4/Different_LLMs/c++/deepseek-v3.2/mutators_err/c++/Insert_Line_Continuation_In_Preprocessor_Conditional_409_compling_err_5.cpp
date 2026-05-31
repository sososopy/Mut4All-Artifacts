//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Line_Continuation_In_Preprocessor_Conditional_409
 */ 
class MutatorFrontendAction_409 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(409)

private:
    class MutatorASTConsumer_409 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_409(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Line_Continuation_In_Preprocessor_Conditional_409.h"

// ========================================================================================================
#define MUT409_OUTPUT 1

void MutatorFrontendAction_409::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<ConditionalDirective>("ConditionalDirective")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto directiveText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Find the condition part after #if, #elif, #ifdef, #ifndef
      size_t condStart = directiveText.find_first_not_of("#ifdef #ifndef #if #elif ");
      if (condStart == std::string::npos) return;
      
      // Identify tokens in the condition expression
      std::string condition = directiveText.substr(condStart);
      // Remove trailing whitespace/newline
      condition = condition.substr(0, condition.find_first_of("\n"));
      
      // Find suitable insertion points for line continuation
      std::vector<size_t> insertionPoints;
      for (size_t i = 0; i < condition.size(); ++i) {
        if (condition[i] == '&' || condition[i] == '|' || condition[i] == '>' || 
            condition[i] == '<' || condition[i] == '=' || condition[i] == '!' ||
            condition[i] == '(' || condition[i] == ')') {
          // Check that we are not splitting a token
          if (i > 0 && condition[i-1] != ' ' && condition[i-1] != '\t') {
            insertionPoints.push_back(i);
          }
        }
      }
      
      if (insertionPoints.empty()) return;
      
      // Randomly choose an insertion point
      size_t idx = getrandom::getRandomIndex(insertionPoints.size() - 1);
      size_t insertPos = insertionPoints[idx];
      
      // Insert backslash-newline sequence
      std::string mutatedCondition = condition.substr(0, insertPos) + " \\\n" + condition.substr(insertPos);
      std::string mutatedDirective = directiveText.substr(0, condStart) + mutatedCondition + directiveText.substr(condStart + condition.length());
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDirective);
    }
}
  
void MutatorFrontendAction_409::MutatorASTConsumer_409::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = conditionalDirective().bind("ConditionalDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
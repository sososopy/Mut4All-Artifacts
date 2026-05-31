//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Requires_Comma_77
 */ 
class MutatorFrontendAction_77 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(77)

private:
    class MutatorASTConsumer_77 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_77(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Requires_Comma_77.h"

// ========================================================================================================
#define MUT77_OUTPUT 1

void MutatorFrontendAction_77::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RE = Result.Nodes.getNodeAs<clang::Expr>("RequiresExpr")) {
      //Filter nodes in header files
      if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto sourceRange = RE->getSourceRange();
      auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      
      //Find the body of the requires-expression
      auto *requiresExpr = dyn_cast<clang::RequiresExpr>(RE);
      if (!requiresExpr) return;
      auto bodyRange = requiresExpr->getBody()->getSourceRange();
      auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);
      
      //Check if there are at least two requirements
      auto requirements = requiresExpr->getRequirements();
      if (requirements.size() < 2) return;
      
      //Find positions between requirements in the source text
      std::vector<SourceLocation> requirementEnds;
      for (unsigned i = 0; i < requirements.size(); ++i) {
          auto req = requirements[i];
          requirementEnds.push_back(req.getSourceRange().getEnd());
      }
      
      //Look for commas between requirements in the original source
      std::vector<bool> hasCommaBetween;
      for (unsigned i = 0; i < requirements.size() - 1; ++i) {
          SourceLocation start = requirementEnds[i];
          SourceLocation end = requirements[i+1].getSourceRange().getBegin();
          
          //Get text between requirements
          auto betweenRange = CharSourceRange::getCharRange(start, end);
          auto betweenText = stringutils::rangetoStr(*(Result.SourceManager), 
              betweenRange.getAsRange());
          
          //Check if there's a comma (possibly with whitespace)
          bool hasComma = false;
          for (char c : betweenText) {
              if (c == ',') {
                  hasComma = true;
                  break;
              }
          }
          hasCommaBetween.push_back(hasComma);
      }
      
      //Randomly choose to insert or remove a comma
      int mutationType = getrandom::getRandomIndex(1); // 0 = insert, 1 = remove
      
      //Find a suitable position for mutation
      int mutationPos = -1;
      if (mutationType == 0) {
          //Insert comma: find a position without comma
          std::vector<int> positionsWithoutComma;
          for (unsigned i = 0; i < hasCommaBetween.size(); ++i) {
              if (!hasCommaBetween[i]) {
                  positionsWithoutComma.push_back(i);
              }
          }
          if (positionsWithoutComma.empty()) return;
          mutationPos = positionsWithoutComma[getrandom::getRandomIndex(positionsWithoutComma.size() - 1)];
      } else {
          //Remove comma: find a position with comma
          std::vector<int> positionsWithComma;
          for (unsigned i = 0; i < hasCommaBetween.size(); ++i) {
              if (hasCommaBetween[i]) {
                  positionsWithComma.push_back(i);
              }
          }
          if (positionsWithComma.empty()) return;
          mutationPos = positionsWithComma[getrandom::getRandomIndex(positionsWithComma.size() - 1)];
      }
      
      //Get the exact location for mutation
      SourceLocation mutationLoc = requirementEnds[mutationPos];
      
      //Perform mutation on the source code text by applying string replacement
      if (mutationType == 0) {
          //Insert comma
          Rewrite.InsertTextAfterToken(mutationLoc, " ,");
      } else {
          //Remove comma - find and remove the comma token
          SourceLocation start = requirementEnds[mutationPos];
          SourceLocation end = requirements[mutationPos+1].getSourceRange().getBegin();
          
          auto betweenRange = CharSourceRange::getCharRange(start, end);
          auto betweenText = stringutils::rangetoStr(*(Result.SourceManager), betweenRange.getAsRange());
          
          //Find the comma position
          size_t commaPos = betweenText.find(',');
          if (commaPos != std::string::npos) {
              //Calculate the location of the comma
              SourceLocation commaLoc = start.getLocWithOffset(commaPos);
              //Remove just the comma (keep whitespace)
              Rewrite.RemoveText(commaLoc, 1);
          }
      }
    }
}
  
void MutatorFrontendAction_77::MutatorASTConsumer_77::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = expr().bind("RequiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Colon_Range_For_Loop_331
 */ 
class MutatorFrontendAction_331 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(331)

private:
    class MutatorASTConsumer_331 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_331(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_331.h"

// ========================================================================================================
#define MUT331_OUTPUT 1

void MutatorFrontendAction_331::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ForStmt>("ForStmt")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto forLoopText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Check if it is a range-based for loop with ternary operator in range-init
      auto Init = MT->getInit();
      if (!Init) return;
      auto Cond = MT->getCond();
      if (!Cond) return;
      auto Inc = MT->getInc();
      if (!Inc) return;
      
      //Check if the init contains a conditional expression (ternary operator)
      if (auto ConditionalOp = dyn_cast<ConditionalOperator>(Init)) {
        //Check for colon separator between ternary result and range expression
        SourceRange InitRange = Init->getSourceRange();
        SourceRange IncRange = Inc->getSourceRange();
        
        //Find the colon separating init and range expression
        SourceLocation ColonLoc;
        //Search between init end and range start
        SourceLocation InitEnd = InitRange.getEnd();
        SourceLocation IncStart = IncRange.getBegin();
        
        //Get source text between init and range
        std::string BetweenText = stringutils::rangetoStr(*(Result.SourceManager),
                                                          SourceRange(InitEnd, IncStart));
        
        //Look for colon in between text
        size_t colonPos = BetweenText.find(':');
        if (colonPos == std::string::npos) return; //No colon found
        
        //Perform mutation on the source code text by applying string replacement
        int mutationType = getrandom::getRandomIndex(1); //0: add extra colon, 1: remove colon
        
        if (mutationType == 0) {
            //Add extra colon after existing colon
            size_t insertPos = colonPos + 1;
            BetweenText.insert(insertPos, ":");
        } else {
            //Remove the colon
            BetweenText.erase(colonPos, 1);
        }
        
        //Construct new for loop text
        std::string InitText = stringutils::rangetoStr(*(Result.SourceManager), InitRange);
        std::string IncText = stringutils::rangetoStr(*(Result.SourceManager), IncRange);
        std::string NewForLoop = "for (" + InitText + BetweenText + IncText + ")";
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), NewForLoop);
      }
    }
}
  
void MutatorFrontendAction_331::MutatorASTConsumer_331::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = forStmt(hasInit(expr().bind("initExpr"))).bind("ForStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
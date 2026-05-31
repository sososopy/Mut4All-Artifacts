//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Member_Template_Function_Call_With_Implicit_Conversion_Context_Omission_195
 */ 
class MutatorFrontendAction_195 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(195)

private:
    class MutatorASTConsumer_195 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_195(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT195_OUTPUT 1

void MutatorFrontendAction_195::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      // Check if it's a member call expression with template arguments
      if (auto *MCE = dyn_cast<clang::MemberExpr>(CE->getCallee())) {
        if (auto *DRE = dyn_cast<clang::DeclRefExpr>(MCE->getMemberDecl())) {
          if (auto *FD = dyn_cast<clang::FunctionDecl>(DRE->getDecl())) {
            if (FD->isTemplateInstantiation() || FD->getPrimaryTemplate()) {
              //Get the source code text of target node
              SourceRange CallRange = CE->getSourceRange();
              SourceLocation LParenLoc = CE->getLParenLoc();
              SourceLocation RParenLoc = CE->getRParenLoc();
              
              if (LParenLoc.isValid() && RParenLoc.isValid()) {
                // Get the text before parentheses (including template arguments)
                SourceLocation BeforeParen = LParenLoc.getLocWithOffset(-1);
                SourceRange BeforeParenRange(CE->getBeginLoc(), BeforeParen);
                std::string BeforeParenText = stringutils::rangetoStr(*(Result.SourceManager), BeforeParenRange);
                
                // Get the text after parentheses
                SourceLocation AfterParen = RParenLoc.getLocWithOffset(1);
                
                //Perform mutation on the1 source2 code text by applying string replacement
                // Remove parentheses and arguments
                SourceRange ArgRange(LParenLoc, RParenLoc);
                Rewrite.RemoveText(ArgRange);
                
                //Replace the original AST node with the mutated one
                // The mutation is already applied by removing the argument range
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_195::MutatorASTConsumer_195::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(
        hasAncestor(
            anyOf(
                ifStmt(),
                binaryOperator(),
                unaryOperator()
            )
        ),
        callee(
            memberExpr(
                hasDescendant(
                    declRefExpr(
                        to(
                            functionDecl(
                                anyOf(
                                    isTemplateInstantiation(),
                                    hasPrimaryTemplate()
                                )
                            )
                        )
                    )
                )
            )
        )
    ).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
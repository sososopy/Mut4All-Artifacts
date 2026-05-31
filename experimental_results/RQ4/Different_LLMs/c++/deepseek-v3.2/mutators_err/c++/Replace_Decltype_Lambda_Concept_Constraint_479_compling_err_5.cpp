//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Decltype_Lambda_Concept_Constraint_479
 */ 
class MutatorFrontendAction_479 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(479)

private:
    class MutatorASTConsumer_479 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_479(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Decltype_Lambda_Concept_Constraint_479.h"

// ========================================================================================================
#define MUT479_OUTPUT 1

void MutatorFrontendAction_479::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAliasDecl")) {
      //Filter nodes in header files
      if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getLocation()))
        return;
      //Get the source code text of target node
      auto aliasText = stringutils::rangetoStr(*(Result.SourceManager),
                                               TA->getSourceRange());
      //Check if the alias uses decltype with a lambda
      auto underlyingType = TA->getUnderlyingType();
      if (!underlyingType.isNull() && underlyingType->isDecltypeType()) {
        const DecltypeType *DT = underlyingType->getAs<DecltypeType>();
        if (DT) {
          Expr *E = DT->getUnderlyingExpr();
          if (E && isa<LambdaExpr>(E)) {
            LambdaExpr *LE = cast<LambdaExpr>(E);
            //Check if lambda has a template parameter with a concept constraint
            if (LE->getTemplateParameterList()) {
              auto *TPL = LE->getTemplateParameterList();
              if (TPL->size() > 0) {
                auto *TP = TPL->getParam(0);
                if (TP->hasAssociatedConstraints()) {
                  //Extract concept name and parameter
                  auto constraintExpr = TP->getTypeConstraintExpr();
                  if (constraintExpr) {
                    auto conceptRange = constraintExpr->getSourceRange();
                    std::string conceptStr = stringutils::rangetoStr(*(Result.SourceManager), conceptRange);
                    //Check if lambda already has a requires-clause
                    Expr *RC = LE->getTrailingRequiresClause();
                    std::string requiresClause = "";
                    if (RC) {
                      auto rcRange = RC->getSourceRange();
                      requiresClause = stringutils::rangetoStr(*(Result.SourceManager), rcRange);
                      //Append new constraint with &&
                      requiresClause += " && " + conceptStr + "<" + TP->getNameAsString() + ">";
                    } else {
                      //Add new requires-clause
                      requiresClause = " requires " + conceptStr + "<" + TP->getNameAsString() + ">";
                    }
                    //Find the lambda's source range
                    auto lambdaRange = LE->getSourceRange();
                    std::string lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
                    //Insert requires-clause before the lambda body
                    auto bodyStart = LE->getBody()->getBeginLoc();
                    std::string mutatedLambda = lambdaText.substr(0, Result.SourceManager->getFileOffset(bodyStart) - Result.SourceManager->getFileOffset(lambdaRange.getBegin())) + requiresClause + lambdaText.substr(Result.SourceManager->getFileOffset(bodyStart) - Result.SourceManager->getFileOffset(lambdaRange.getBegin()));
                    //Replace the lambda in the alias text
                    std::string mutatedAlias = aliasText;
                    mutatedAlias.replace(Result.SourceManager->getFileOffset(lambdaRange.getBegin()) - Result.SourceManager->getFileOffset(TA->getSourceRange().getBegin()), lambdaText.length(), mutatedLambda);
                    //Replace the original AST node with the mutated one
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(TA->getSourceRange()), mutatedAlias);
                  }
                }
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_479::MutatorASTConsumer_479::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasDecl(hasUnderlyingType(decltypeType())).bind("TypeAliasDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
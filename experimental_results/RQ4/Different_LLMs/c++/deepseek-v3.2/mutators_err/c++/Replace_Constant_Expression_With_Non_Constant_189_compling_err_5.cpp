//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constant_Expression_With_Non_Constant_189
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
        std::vector<std::pair<const clang::VarDecl *, const clang::Expr *>> candidateReplacements;
    };
};

//source file
#include "../include/Mutator_Replace_Constant_Expression_With_Non_Constant_189.h"

// ========================================================================================================
#define MUT189_OUTPUT 1

void MutatorFrontendAction_189::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::Expr>("ConstantExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getExprLoc()))
        return;
      //Check if expression is constant
      if (!CE->isEvaluatable(*Result.Context))
        return;
      //Get the source code text of target node
      std::string exprText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
      //(optional)Record the node information to be used in the mutation process
      candidateReplacements.push_back(std::make_pair(nullptr, CE));
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("NonConstVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if variable is non-const and integral type
      if (VD->getType()->isIntegralOrEnumerationType() && !VD->getType().isConstQualified()) {
        //Record candidate non-const variables
        for (auto &pair : candidateReplacements) {
          if (pair.first == nullptr) {
            pair.first = VD;
          }
        }
      }
    }
    else if (auto *TA = Result.Nodes.getNodeAs<clang::TemplateArgumentLoc>("TemplateArg")) {
      //Filter nodes in header files
      if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getLocation()))
        return;
      //Check if template argument is integral constant
      if (TA->getArgument().getKind() != clang::TemplateArgument::Integral)
        return;
      //Find a candidate replacement
      for (auto &pair : candidateReplacements) {
        if (pair.first != nullptr && pair.second != nullptr) {
          //Perform mutation on the source code text by applying string replacement
          std::string replacement = pair.first->getNameAsString();
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(pair.second->getSourceRange()), replacement);
          candidateReplacements.clear();
          return;
        }
      }
      //If no non-const variable found, create one
      std::string varName = "mut_189_var_" + std::to_string(TA->getArgument().getAsIntegral().getExtValue());
      std::string varDecl = "int " + varName + " = " + std::to_string(TA->getArgument().getAsIntegral().getExtValue()) + ";";
      Rewrite.InsertTextBefore(TA->getLocation(),  varDecl);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TA->getSourceRange()), varName);
    }
}
  
void MutatorFrontendAction_189::MutatorASTConsumer_189::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto constExprMatcher = expr(isConstantExpr()).bind("ConstantExpr");
    auto nonConstVarMatcher = varDecl(hasType(isInteger()), unless(hasType(isConstQualified()))).bind("NonConstVar");
    auto templateArgMatcher = templateArgumentLoc(hasArgument(isIntegral())).bind("TemplateArg");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(constExprMatcher, &callback);
    matchFinder.addMatcher(nonConstVarMatcher, &callback);
    matchFinder.addMatcher(templateArgMatcher, &callback);
    matchFinder.matchAST(Context);
}
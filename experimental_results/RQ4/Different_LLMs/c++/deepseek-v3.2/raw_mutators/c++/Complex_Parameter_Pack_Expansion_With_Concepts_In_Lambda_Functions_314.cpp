//header file
#pragma once
#include "Mutator_base.h"

/**
 * Complex_Parameter_Pack_Expansion_With_Concepts_In_Lambda_Functions_314
 */ 
class MutatorFrontendAction_314 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(314)

private:
    class MutatorASTConsumer_314 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_314(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> targetFunctions;
    };
};

//source file
#include "../include/Complex_Parameter_Pack_Expansion_With_Concepts_In_Lambda_Functions_314.h"

// ========================================================================================================
#define MUT314_OUTPUT 1

void MutatorFrontendAction_314::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if it's a template function with a lambda containing parameter pack
      if (!FD->isTemplate() || !FD->hasBody())
        return;
      
      //Record candidate functions for later processing
      targetFunctions.push_back(FD);
    }
    else if (auto *LD = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaWithPack")) {
      //Filter nodes in header files
      if (!LD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LD->getLocation()))
        return;
      
      //Find the enclosing template function
      const clang::FunctionDecl* enclosingFunc = nullptr;
      for (const auto* func : targetFunctions) {
        if (LD->getBeginLoc() >= func->getBeginLoc() && LD->getEndLoc() <= func->getEndLoc()) {
          enclosingFunc = func;
          break;
        }
      }
      if (!enclosingFunc)
        return;
      
      //Get the source code text of target node
      auto lambdaRange = CharSourceRange::getTokenRange(LD->getSourceRange());
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      
      //Check if lambda has parameter pack (via std::index_sequence or similar)
      bool hasPack = false;
      auto callExpr = LD->getCallOperator();
      if (callExpr && callExpr->getNumParams() > 0) {
        for (unsigned i = 0; i < callExpr->getNumParams(); ++i) {
          auto param = callExpr->getParamDecl(i);
          if (param->getType()->isPackExpansionType()) {
            hasPack = true;
            break;
          }
        }
      }
      if (!hasPack)
        return;
      
      //Perform mutation on the source code text by applying string replacement
      //First, add concept declaration before the function if not already present
      SourceLocation funcStart = enclosingFunc->getBeginLoc();
      std::string conceptDecl = "\ntemplate <typename T, std::size_t S> concept placeholder = true;\n";
      
      //Check if concept already exists in scope
      bool conceptExists = false;
      auto parentDC = enclosingFunc->getDeclContext();
      for (auto decl : parentDC->decls()) {
        if (auto* TD = dyn_cast<TemplateDecl>(decl)) {
          if (TD->getTemplatedDecl() && TD->getTemplatedDecl()->getNameAsString() == "placeholder") {
            conceptExists = true;
            break;
          }
        }
      }
      
      if (!conceptExists) {
        Rewrite.InsertText(funcStart, conceptDecl);
      }
      
      //Modify lambda body to include inner lambda with concept constraint
      //Find the lambda body location
      auto bodyRange = CharSourceRange::getTokenRange(LD->getBody()->getSourceRange());
      auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);
      
      //Create inner lambda with constrained parameter pack expansion
      std::string innerLambda = "[](placeholder<Is> auto..., auto extra) {} (1, 2);";
      
      //Insert inner lambda into the lambda body
      size_t bodyStartPos = bodyText.find("{");
      if (bodyStartPos != std::string::npos) {
        std::string mutatedBody = bodyText;
        mutatedBody.insert(bodyStartPos + 1, "\nreturn " + innerLambda + "\n");
        
        //Replace the entire lambda
        std::string mutatedLambda = lambdaText;
        size_t lambdaBodyPos = mutatedLambda.find(bodyText);
        if (lambdaBodyPos != std::string::npos) {
          mutatedLambda.replace(lambdaBodyPos, bodyText.length(), mutatedBody);
          
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(lambdaRange, mutatedLambda);
        }
      }
    }
}
  
void MutatorFrontendAction_314::MutatorASTConsumer_314::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher funcMatcher = functionDecl(isTemplate(), hasBody(has(lambdaExpr().bind("LambdaWithPack")))).bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}
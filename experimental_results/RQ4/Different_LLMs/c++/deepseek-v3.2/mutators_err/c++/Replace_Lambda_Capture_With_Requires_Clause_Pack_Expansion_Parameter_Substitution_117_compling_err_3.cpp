//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Capture_With_Requires_Clause_Pack_Expansion_Parameter_Substitution_117
 */ 
class MutatorFrontendAction_117 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(117)

private:
    class MutatorASTConsumer_117 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_117(Rewriter &R) : TheRewriter(R) {}
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
        std::string derivedType;
        bool hasContainerCapture;
    };
};

//source file
#include "../include/Mutator_Replace_Lambda_Capture_With_Requires_Clause_Pack_Expansion_Parameter_Substitution_117.h"

// ========================================================================================================
#define MUT117_OUTPUT 1

void MutatorFrontendAction_117::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("targetLambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaRange = MT->getSourceRange();
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      
      //Check if lambda has a requires clause with pack expansion
      auto callOperator = MT->getCallOperator();
      if (!callOperator || !callOperator->hasBody())
        return;
      
      //Look for requires clause in the lambda
      bool hasRequiresClause = false;
      std::string requiresText;
      SourceLocation requiresLoc;
      
      //Check if lambda has trailing requires clause
      if (auto *TRD = dyn_cast<FunctionDecl>(callOperator)) {
        if (TRD->getTrailingRequiresClause()) {
          hasRequiresClause = true;
          requiresLoc = TRD->getTrailingRequiresClause()->getBeginLoc();
          requiresText = stringutils::rangetoStr(*(Result.SourceManager), 
            TRD->getTrailingRequiresClause()->getSourceRange());
        }
      }
      
      if (!hasRequiresClause)
        return;
      
      //Check for pack expansion in requires clause
      if (requiresText.find("...") == std::string::npos)
        return;
      
      //Analyze lambda captures for container types
      hasContainerCapture = false;
      derivedType = "int"; // Default fallback type
      
      for (auto capture : MT->captures()) {
        if (capture.capturesVariable()) {
          auto var = capture.getCapturedVar();
          if (var) {
            auto varType = var->getType();
            //Check if type is a container (simplified check for std::vector, std::array, etc.)
            if (varType.getAsString().find("std::vector") != std::string::npos ||
                varType.getAsString().find("std::array") != std::string::npos ||
                varType.getAsString().find("std::deque") != std::string::npos) {
              hasContainerCapture = true;
              
              //Extract element type from container
              auto templateArgs = varType->getAs<TemplateSpecializationType>();
              if (templateArgs && templateArgs->getNumArgs() > 0) {
                auto elementType = templateArgs->getArg(0);
                if (elementType.getKind() == TemplateArgument::Type) {
                  derivedType = elementType.getAsType().getAsString();
                  //Handle nested containers
                  size_t pos = derivedType.find("std::vector<");
                  while (pos != std::string::npos) {
                    size_t end = derivedType.find_last_of('>');
                    if (end != std::string::npos) {
                      derivedType = derivedType.substr(pos + 12, end - pos - 12);
                    }
                    pos = derivedType.find("std::vector<");
                  }
                }
              }
              break;
            }
          }
        }
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = lambdaText;
      
      //Replace pack expansion with single parameter
      size_t packPos = mutatedText.find("params...");
      if (packPos != std::string::npos) {
        std::string replacement;
        if (hasContainerCapture) {
          replacement = "std::declval<" + derivedType + ">()";
        } else {
          replacement = "param";
        }
        mutatedText.replace(packPos, 9, replacement);
      }
      
      //Replace parameter pack declaration
      size_t autoParamsPos = mutatedText.find("auto... params");
      if (autoParamsPos != std::string::npos) {
        std::string replacement;
        if (hasContainerCapture) {
          replacement = derivedType + " param";
        } else {
          replacement = "int param";
        }
        mutatedText.replace(autoParamsPos, 14, replacement);
      }
      
      //Add missing includes if needed
      if (hasContainerCapture && derivedType.find("std::") != std::string::npos) {
        if (mutatedText.find("#include") == std::string::npos) {
          mutatedText = "/*mut117*/#include <utility>\n" + mutatedText;
        }
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), mutatedText);
    }
}
  
void MutatorFrontendAction_117::MutatorASTConsumer_117::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("targetLambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(mat2cher, &callback);
    matchFinder.matchAST(Context);
}
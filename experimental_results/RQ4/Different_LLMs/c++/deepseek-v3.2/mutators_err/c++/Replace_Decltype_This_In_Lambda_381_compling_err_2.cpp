//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Decltype_This_In_Lambda_381
 */ 
class MutatorFrontendAction_381 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(381)

private:
    class MutatorASTConsumer_381 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_381(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> availableTypes;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT381_OUTPUT 1

void MutatorFrontendAction_381::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Check if the lambda contains a decltype with 'this'
      bool hasDecltypeThis = false;
      size_t decltypePos = lambdaText.find("decltype");
      while (decltypePos != std::string::npos) {
        size_t thisPos = lambdaText.find("this", decltypePos);
        if (thisPos != std::string::npos && thisPos > decltypePos) {
          hasDecltypeThis = true;
          break;
        }
        decltypePos = lambdaText.find("decltype", decltypePos + 1);
      }
      if (!hasDecltypeThis) return;

      //Check lambda parameters
      auto lambdaDecl = MT->getLambdaClass();
      if (!lambdaDecl) return;
      auto callOperator = lambdaDecl->getLambdaCallOperator();
      if (!callOperator) return;
      
      std::string paramName = "";
      std::string paramType = "";
      if (callOperator->getNumParams() > 0) {
        auto firstParam = callOperator->getParamDecl(0);
        paramName = firstParam->getNameAsString();
        paramType = firstParam->getType().getAsString();
      } else {
        //No parameters, need to add one
        if (availableTypes.empty()) {
          //Collect available types from the translation unit
          auto &SM = Result.Context->getSourceManager();
          auto decls = Result.Context->getTranslationUnitDecl()->decls();
          for (auto decl : decls) {
            if (auto *TD = dyn_cast<TypeDecl>(decl)) {
              if (TD->getLocation().isValid() && SM.isWrittenInMainFile(TD->getLocation())) {
                std::string typeName = TD->getNameAsString();
                if (typeName != "" && typeName != "auto") {
                  availableTypes.push_back(typeName);
                }
              }
            }
          }
          if (availableTypes.empty()) {
            paramType = "int";
          } else {
            size_t idx = getrandom::getRandomIndex(availableTypes.size() - 1);
            paramType = availableTypes[idx];
          }
        } else {
          size_t idx = getrandom::getRandomIndex(availableTypes.size() - 1);
          paramType = availableTypes[idx];
        }
        paramName = "mut_param_" + std::to_string(getrandom::getRandomIndex(1000));
      }

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = lambdaText;
      size_t decltypePos2 = mutatedText.find("decltype");
      while (decltypePos2 != std::string::npos) {
        size_t thisPos2 = mutatedText.find("this", decltypePos2);
        if (thisPos2 != std::string::npos && thisPos2 > decltypePos2) {
          mutatedText.replace(thisPos2, 4, paramName);
          decltypePos2 = mutatedText.find("decltype", decltypePos2 + 1);
        } else {
          decltypePos2 = mutatedText.find("decltype", decltypePos2 + 1);
        }
      }

      //If no parameters existed, add parameter list
      if (callOperator->getNumParams() == 0) {
        size_t lambdaStart = mutatedText.find('[');
        size_t lambdaEnd = mutatedText.find(']', lambdaStart);
        if (lambdaEnd != std::string::npos) {
          mutatedText.insert(lambdaEnd + 1, "(" + paramType + " " + paramName + ")");
        }
      }

      mutatedText = "/*mut381*/" + mutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_381::MutatorASTConsumer_381::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
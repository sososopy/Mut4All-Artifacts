//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mismatch_Nested_Generic_Lambda_48
 */ 
class MutatorFrontendAction_48 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(48)

private:
    class MutatorASTConsumer_48 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_48(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::LambdaExpr*> outerLambdas;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT48_OUTPUT 1

void MutatorFrontendAction_48::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("outerLambda")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getLocation()))
        return;
      
      // Check if this lambda is generic (has template parameter list)
      auto *Method = LE->getCallOperator();
      if (!Method || !Method->getDescribedFunctionTemplate())
        return;
      
      // Check if body contains another lambda
      bool hasInnerLambda = false;
      auto *Body = Method->getBody();
      if (Body) {
        for (auto *Child : Body->children()) {
          if (llvm::isa<clang::LambdaExpr>(Child)) {
            hasInnerLambda = true;
            break;
          }
        }
      }
      
      if (hasInnerLambda) {
        outerLambdas.push_back(LE);
      }
    }
    else if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("innerLambda")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getLocation()))
        return;
      
      // Find if this inner lambda belongs to one of our recorded outer lambdas
      const clang::LambdaExpr* parentOuter = nullptr;
      for (auto* outer : outerLambdas) {
        auto* outerMethod = outer->getCallOperator();
        if (!outerMethod || !outerMethod->getBody()) continue;
        
        // Check if inner lambda is within outer's body
        if (Result.Context->getSourceManager().isPointWithin(
            LE->getBeginLoc(), 
            outerMethod->getBody()->getBeginLoc(), 
            outerMethod->getBody()->getEndLoc())) {
          parentOuter = outer;
          break;
        }
      }
      
      if (!parentOuter) return;
      
      // Get the source code text of target node
      std::string lambdaText = stringutils::rangetoStr(*(Result.SourceManager),
                                                       LE->getSourceRange());
      
      // Check if inner lambda is also generic
      auto* innerMethod = LE->getCallOperator();
      if (!innerMethod || !innerMethod->getDescribedFunctionTemplate())
        return;
      
      // Perform mutation: transform inner lambda
      // Find the parameter list start
      size_t paramStart = lambdaText.find('(');
      if (paramStart == std::string::npos) return;
      
      // Build new template parameter list with pack
      std::string newTemplateParams = "<class U1, class... Us>";
      
      // Build new parameter list with single int parameter
      std::string newParams = "(int)";
      
      // Add requires clause
      std::string requiresClause = " requires (sizeof...(Us) >= 0)";
      
      // Construct mutated lambda
      std::string mutatedLambda;
      
      // Find capture list (everything up to first '[' after potential leading whitespace)
      size_t lambdaStart = 0;
      while (lambdaStart < lambdaText.size() && std::isspace(lambdaText[lambdaStart])) 
        lambdaStart++;
      
      if (lambdaText[lambdaStart] != '[') return;
      
      size_t captureEnd = lambdaText.find(']', lambdaStart);
      if (captureEnd == std::string::npos) return;
      
      std::string capturePart = lambdaText.substr(lambdaStart, captureEnd - lambdaStart + 1);
      
      // Check for mutable/constexpr specifiers
      std::string specifiers;
      size_t afterCapture = captureEnd + 1;
      while (afterCapture < lambdaText.size() && 
             (lambdaText.substr(afterCapture, 8) == "mutable " ||
              lambdaText.substr(afterCapture, 9) == "constexpr ")) {
        size_t nextSpace = lambdaText.find(' ', afterCapture);
        if (nextSpace == std::string::npos) break;
        specifiers += lambdaText.substr(afterCapture, nextSpace - afterCapture + 1);
        afterCapture = nextSpace + 1;
      }
      
      // Check for exception specifier
      std::string exceptionSpec;
      if (afterCapture < lambdaText.size() && lambdaText[afterCapture] == 'n' &&
          lambdaText.substr(afterCapture, 6) == "noexcept") {
        size_t noexceptEnd = lambdaText.find('(', afterCapture);
        if (noexceptEnd != std::string::npos) {
          exceptionSpec = lambdaText.substr(afterCapture, noexceptEnd - afterCapture);
          afterCapture = noexceptEnd;
        }
      }
      
      // Check for trailing return type
      std::string trailingReturn;
      if (afterCapture < lambdaText.size() && lambdaText[afterCapture] == '-' &&
          lambdaText.substr(afterCapture, 2) == "->") {
        size_t arrowEnd = lambdaText.find('{', afterCapture);
        if (arrowEnd != std::string::npos) {
          trailingReturn = lambdaText.substr(afterCapture, arrowEnd - afterCapture);
          afterCapture = arrowEnd;
        }
      }
      
      // Find the body
      size_t bodyStart = lambdaText.find('{', afterCapture);
      if (bodyStart == std::string::npos) return;
      
      std::string body = lambdaText.substr(bodyStart);
      
      // Construct new lambda
      mutatedLambda = capturePart + " " + specifiers + newTemplateParams + " " + 
                     newParams + exceptionSpec + requiresClause + trailingReturn + body;
      
      // Also need to create a decltype alias that calls with wrong number of arguments
      // We'll insert this after the outer lambda's declaration
      auto* outerMethod = parentOuter->getCallOperator();
      if (!outerMethod) return;
      
      // Get outer lambda's end location
      auto outerEnd = parentOuter->getEndLoc();
      
      // Create alias template that tries to call inner lambda with zero arguments
      std::string aliasDecl = "\nusing MismatchAlias_48 = decltype(";
      
      // Need to generate a call expression for the outer lambda to get the inner one
      // For simplicity, we'll use a default-constructed argument if we can determine the type
      auto* outerTemplate = outerMethod->getDescribedFunctionTemplate();
      if (outerTemplate && outerTemplate->getTemplateParameters()->size() > 0) {
        // Use first template parameter with a default value
        aliasDecl += parentOuter->getLambdaClass()->getNameAsString() + "{}";
      } else {
        // Fallback: use int literal
        aliasDecl += std::to_string(getrandom::getRandomIndex(100));
      }
      
      aliasDecl += "());  // Mismatch: inner lambda expects int but called with no arguments\n";
      
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), mutatedLambda);
      
      // Insert the alias declaration after the outer lambda
      Rewrite.InsertTextAfterToken(outerEnd, aliasDecl);
    }
}
  
void MutatorFrontendAction_48::MutatorASTConsumer_48::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    // First matcher for outer generic lambdas
    DeclarationMatcher outerMatcher = lambdaExpr(
        hasDescendant(lambdaExpr().bind("innerLambda"))
    ).bind("outerLambda");
    
    // Second matcher for inner generic lambdas
    DeclarationMatcher innerMatcher = lambdaExpr(
        hasAncestor(lambdaExpr(
            has(decl(hasDescendant(functionDecl(
                hasDescendant(lambdaExpr().bind("innerLambda"))
            )))
        ))
    )).bind("innerLambda");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(outerMatcher, &callback);
    matchFinder.addMatcher(innerMatcher, &callback);
    matchFinder.matchAST(Context);
}
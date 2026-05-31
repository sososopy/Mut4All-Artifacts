//source file
#include "../include/Mutator_Replace_Generic_Lambda_In_Requires_Clause_316.h"

// ========================================================================================================
#define MUT316_OUTPUT 1

void MutatorFrontendAction_316::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::Expr>("RequiresExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getExprLoc()))
        return;
      //Get the source code text of target node
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      
      //Collect available type names from the surrounding context
      availableTypeNames.clear();
      const DeclContext* context = Result.Context->getTranslationUnitDecl();
      if (context) {
        for (const Decl* decl : context->decls()) {
          if (const TypedefNameDecl* typedefDecl = dyn_cast<TypedefNameDecl>(decl)) {
            availableTypeNames.push_back(typedefDecl->getNameAsString());
          } else if (const TypeDecl* typeDecl = dyn_cast<TypeDecl>(decl)) {
            availableTypeNames.push_back(typeDecl->getNameAsString());
          }
        }
      }
      
      //Look for generic lambda expressions within the requires-clause
      bool hasGenericLambda = false;
      std::string mutatedText = originalText;
      size_t genericLambdaPos = mutatedText.find("[]<");
      if (genericLambdaPos != std::string::npos) {
        hasGenericLambda = true;
        
        //Find the template parameter list
        size_t templateEnd = mutatedText.find(">", genericLambdaPos);
        if (templateEnd != std::string::npos) {
          //Extract the template parameter
          std::string templateParam = mutatedText.substr(genericLambdaPos + 3, templateEnd - genericLambdaPos - 3);
          
          //Find the lambda parameter list
          size_t paramStart = mutatedText.find("(", templateEnd);
          size_t paramEnd = mutatedText.find(")", paramStart);
          if (paramStart != std::string::npos && paramEnd != std::string::npos) {
            //Replace template parameter with a concrete type if available
            if (!availableTypeNames.empty()) {
              size_t randomIndex = getrandom::getRandomIndex(availableTypeNames.size() - 1);
              std::string concreteType = availableTypeNames[randomIndex];
              
              //Remove the template parameter from the lambda
              mutatedText.erase(genericLambdaPos + 1, templateEnd - genericLambdaPos + 1);
              
              //Replace references to the template parameter in the parameter list
              std::string paramList = mutatedText.substr(paramStart, paramEnd - paramStart + 1);
              size_t templateRefPos = paramList.find(templateParam);
              while (templateRefPos != std::string::npos) {
                paramList.replace(templateRefPos, templateParam.length(), concreteType);
                templateRefPos = paramList.find(templateParam, templateRefPos + concreteType.length());
              }
              mutatedText.replace(paramStart, paramEnd - paramStart + 1, paramList);
            } else {
              //If no concrete type available, try to replace with a simple expression
              //Find the variable used in the lambda call
              size_t callStart = mutatedText.find("}(t)", paramEnd);
              if (callStart != std::string::npos) {
                //Replace the entire lambda expression with a simple member access
                std::string replacement = "t.x";
                mutatedText.replace(genericLambdaPos, callStart + 4 - genericLambdaPos, replacement);
              }
            }
          }
        }
      }
      
      //Perform mutation on the source code text by applying string replacement
      if (hasGenericLambda) {
        mutatedText = "/*mut316*/" + mutatedText;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
      }
    }
}
  
void MutatorFrontendAction_316::MutatorASTConsumer_316::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = expr().bind("RequiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
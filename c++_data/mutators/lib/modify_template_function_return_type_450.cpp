//source file
#include "../include/modify_template_function_return_type_450.h"

// ========================================================================================================
#define MUT450_OUTPUT 1

void MutatorFrontendAction_450::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunction")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      //Get the source code text of target node
      auto returnType = MT->getReturnType();
      if (const auto *templateType = returnType->getAs<TemplateSpecializationType>()) {
        if (templateType->getNumArgs() > 0) {
          const auto &firstArg = templateType->getArg(0);
          if (firstArg.getKind() == clang::TemplateArgument::Type) {
            auto originalType = firstArg.getAsType();
            if (originalType->isBuiltinType()) {
              std::string newType = "float"; // Change to a different basic type
              std::string originalTypeStr = originalType.getAsString();
              std::string returnTypeStr = returnType.getAsString();
              
              //Perform mutation on the source code text by applying string replacement
              size_t pos = returnTypeStr.find(originalTypeStr);
              if (pos != std::string::npos) {
                  returnTypeStr.replace(pos, originalTypeStr.length(), newType);
              }
              
              //Replace the original AST node with the mutated one
              SourceRange returnTypeRange = MT->getReturnTypeSourceRange();
              Rewrite.ReplaceText(returnTypeRange, returnTypeStr);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_450::MutatorASTConsumer_450::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), hasParent(cxxRecordDecl(isTemplateInstantiation()))).bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
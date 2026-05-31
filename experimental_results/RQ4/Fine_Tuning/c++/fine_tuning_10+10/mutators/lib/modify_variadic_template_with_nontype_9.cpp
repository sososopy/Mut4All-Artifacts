//source file
#include "../include/modify_variadic_template_with_nontype_9.h"

// ========================================================================================================
#define MUT9_OUTPUT 1

void MutatorFrontendAction_9::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      //Filter nodes in header files
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      
      if (CT->getTemplateParameters()->size() < 1)
        return;

      //Get the source code text of target node
      auto classDecl = CT->getTemplatedDecl();
      auto className = classDecl->getNameAsString();
      auto sourceRange = classDecl->getSourceRange();
      auto sourceCode = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      
      //Perform mutation on the source code text by applying string replacement
      std::string newTemplateParam = "template<int N, ";
      std::string oldTemplateParam = "template<";
      size_t pos = sourceCode.find(oldTemplateParam);
      if (pos != std::string::npos) {
          sourceCode.replace(pos, oldTemplateParam.length(), newTemplateParam);
      }

      // Utilize the non-type parameter N in a context-sensitive manner
      std::string usageInsertion = "\nstatic_assert(N >= 0, \"N must be non-negative\");\n";
      size_t classBodyPos = sourceCode.find("{");
      if (classBodyPos != std::string::npos) {
          sourceCode.insert(classBodyPos + 1, usageInsertion);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceCode);
    }
}
  
void MutatorFrontendAction_9::MutatorASTConsumer_9::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
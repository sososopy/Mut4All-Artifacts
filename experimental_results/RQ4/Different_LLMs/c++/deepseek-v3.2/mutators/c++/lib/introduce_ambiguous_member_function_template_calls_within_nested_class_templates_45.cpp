//source file
#include "../include/Mutator_45.h"

// ========================================================================================================
#define MUT45_OUTPUT 1

void MutatorFrontendAction_45::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("OuterClass")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      if (!CD->isCompleteDefinition())
        return;
      if (!CD->isTemplated())
        return;
      //Record the node information to be used in the mutation process
      outerClasses.push_back(CD);
    }
    else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("InnerClass")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      if (!CD->isCompleteDefinition())
        return;
      if (!CD->isTemplated())
        return;
      //Get the source code text of target node
      auto outerIt = outerClasses.begin();
      while (outerIt != outerClasses.end()) {
        const clang::CXXRecordDecl *outer = *outerIt;
        if (outer->getQualifiedNameAsString() == CD->getQualifiedNameAsString()) {
          outerIt = outerClasses.erase(outerIt);
          continue;
        }
        ++outerIt;
      }
      if (outerClasses.empty())
        return;
      size_t index = getrandom::getRandomIndex(outerClasses.size() - 1);
      const clang::CXXRecordDecl *outer = outerClasses[index];
      //Perform mutation on the source code text by applying string replacement
      std::string insertText = "\n    template<typename V>\n    class Inner_45 {\n        void example_45() {\n            T val = T{};\n            auto result = memberFunc_45(val);\n        }\n    };\n";
      std::string memberFuncText = "\n    template<typename U>\n    U memberFunc_45(U param) { return param; }\n";
      std::string outerContent = stringutils::rangetoStr(*(Result.SourceManager), outer->getSourceRange());
      size_t insertPos = outerContent.rfind('}');
      if (insertPos == std::string::npos)
        return;
      outerContent.insert(insertPos, memberFuncText + insertText);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(outer->getSourceRange()), outerContent);
    }
}
  
void MutatorFrontendAction_45::MutatorASTConsumer_45::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher outerMatcher = cxxRecordDecl(isTemplateInstantiation()).bind("OuterClass");
    DeclarationMatcher innerMatcher = cxxRecordDecl(isTemplateInstantiation(), unless(hasParent(cxxRecordDecl()))).bind("InnerClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(outerMatcher, &callback);
    matchFinder.addMatcher(innerMatcher, &callback);
    matchFinder.matchAST(Context);
}
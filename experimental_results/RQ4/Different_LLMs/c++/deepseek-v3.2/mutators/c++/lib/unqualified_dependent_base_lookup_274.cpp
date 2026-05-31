//source file
#include "../include/Mutator_Unqualified_Dependent_Base_Lookup_274.h"

// ========================================================================================================
#define MUT274_OUTPUT 1

void MutatorFrontendAction_274::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateDerived")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      if (!TD->isCompleteDefinition())
        return;
      //Check if it's a template class with dependent base
      if (!TD->getDescribedClassTemplate())
        return;
      //Record candidate dependent base classes
      dependentBaseClasses.push_back(TD);
    }
    else if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("MethodInDerived")) {
      //Filter nodes in header files
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;
      if (!MD->hasBody())
        return;
      //Find if this method belongs to one of the recorded dependent base template classes
      const clang::CXXRecordDecl* parentClass = MD->getParent();
      bool isTargetClass = false;
      for (const auto* baseClass : dependentBaseClasses) {
        if (parentClass == baseClass) {
          isTargetClass = true;
          break;
        }
      }
      if (!isTargetClass)
        return;
      //Get the source code text of target node
      auto methodRange = CharSourceRange::getTokenRange(MD->getSourceRange());
      std::string methodText = stringutils::rangetoStr(*(Result.SourceManager), MD->getSourceRange());
      //Look for qualified accesses to base members (this-> or Base<T>::)
      //We'll search for patterns and replace them with unqualified access
      bool mutated = false;
      std::string mutatedText = methodText;
      //Find all member expressions in the method body
      if (MD->getBody()) {
        //We'll use a simple approach: find "this->" patterns and remove them
        //This is a simplistic implementation; a more robust one would traverse AST
        size_t pos = 0;
        while ((pos = mutatedText.find("this->", pos)) != std::string::npos) {
          //Check if this is inside the method body (not in parameter declarations etc.)
          size_t bodyStart = mutatedText.find("{");
          size_t bodyEnd = mutatedText.rfind("}");
          if (pos > bodyStart && pos < bodyEnd) {
            //Remove "this->"
            mutatedText.erase(pos, 6);
            mutated = true;
          }
          pos += 1; //Continue searching
        }
        //Also look for Base<T>:: patterns (need to identify base class name)
        //For simplicity, we'll just target "this->" removals
      }
      //If no qualified access found, try to insert an unqualified call to a plausible base member
      if (!mutated) {
        //Find a plausible base member name - we'll use "value" as a common name
        //Insert a call or reference at a random location in the method body
        size_t bodyStart = mutatedText.find("{");
        size_t bodyEnd = mutatedText.rfind("}");
        if (bodyStart != std::string::npos && bodyEnd != std::string::npos && bodyEnd > bodyStart) {
          //Choose a random insertion point within the body
          size_t insertPos = bodyStart + 1 + getrandom::getRandomIndex(bodyEnd - bodyStart - 2);
          //Insert a plausible unqualified access
          std::string insertion = "\n    value;\n";
          mutatedText.insert(insertPos, insertion);
          mutated = true;
        }
      }
      //Perform mutation on the source code text by applying string replacement
      if (mutated) {
        mutatedText = "/*mut274*/" + mutatedText;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(methodRange, mutatedText);
      }
    }
}
  
void MutatorFrontendAction_274::MutatorASTConsumer_274::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    //First find template classes with dependent bases
    DeclarationMatcher templateMatcher = cxxRecordDecl(
        hasAncestor(classTemplateDecl()),
        isDefinition()
    ).bind("TemplateDerived");
    //Then find methods within those classes
    DeclarationMatcher methodMatcher = cxxMethodDecl(
        ofClass(cxxRecordDecl(
            hasAncestor(classTemplateDecl()),
            isDefinition()
        )),
        isDefinition()
    ).bind("MethodInDerived");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(methodMatcher, &callback);
    matchFinder.matchAST(Context);
}
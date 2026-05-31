//source file
#include "../include/Mutator_Add_Template_Member_Function_In_Nested_Specialized_Class_450.h"

// ========================================================================================================
#define MUT450_OUTPUT 1

void MutatorFrontendAction_450::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NC = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("NestedClass")) {
      //Filter nodes in header files
      if (!NC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NC->getLocation()))
        return;
      //Check if it's a nested class within a specialized template
      if (NC->isCompleteDefinition() && NC->getParent() && dyn_cast<clang::ClassTemplateDecl>(NC->getParent())) {
        //Record the node information to be used in the mutation process
        nestedClasses.push_back(NC);
      }
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("CandidateFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record candidate functions for adding calls
      if (FD->hasBody() && FD->getNameAsString() == "main") {
        candidateFunctions.push_back(FD);
      }
    }
    else if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("Specialization")) {
      //Filter nodes in header files
      if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TS->getLocation()))
        return;
      //Get the source code text of target node
      if (TS->isCompleteDefinition()) {
        //Find nested classes within this specialization
        for (auto *Child : TS->decls()) {
          if (auto *RC = dyn_cast<CXXRecordDecl>(Child)) {
            if (RC->isCompleteDefinition()) {
              //Perform mutation on the source code text by applying string replacement
              std::string insertion;
              //Check if std::array is available in context
              bool hasStdArray = false;
              for (auto *Decl : Result.Context->getTranslationUnitDecl()->decls()) {
                if (auto *NS = dyn_cast<NamespaceDecl>(Decl)) {
                  if (NS->getNameAsString() == "std") {
                    for (auto *StdDecl : NS->decls()) {
                      if (auto *TD = dyn_cast<TemplateDecl>(StdDecl)) {
                        if (TD->getNameAsString() == "array") {
                          hasStdArray = true;
                          break;
                        }
                      }
                    }
                  }
                }
              }
              if (hasStdArray) {
                insertion = "\npublic:\n\ttemplate<int N>\n\tstd::array<int, N> new_func() {\n\t\treturn std::array<int, N>{};\n\t}\n";
              } else {
                //Create a dummy template if std::array not available
                insertion = "\npublic:\n\ttemplate<int N>\n\tstruct dummy_array { int data[N]; };\n\ttemplate<int N>\n\tdummy_array<N> new_func() {\n\t\treturn dummy_array<N>{};\n\t}\n";
              }
              //Replace the original AST node with the mutated one
              Rewrite.InsertTextAfterToken(RC->getEndLoc(), insertion);
              //Also add a call in a candidate function if available
              if (!candidateFunctions.empty()) {
                size_t idx = getrandom::getRandomIndex(candidateFunctions.size() - 1);
                auto *CF = candidateFunctions[idx];
                std::string callText = "\n\t/*mut450*/" + TS->getNameAsString() + "::" + RC->getNameAsString() + "{}." + "new_func<42>();\n";
                Rewrite.InsertTextBefore(CF->getBody()->getBeginLoc().getLocWithOffset(1), callText);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_450::MutatorASTConsumer_450::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher nestedMatcher = cxxRecordDecl(isClass(), hasParent(classTemplateDecl())).bind("NestedClass");
    DeclarationMatcher specMatcher = classTemplateSpecializationDecl().bind("Specialization");
    DeclarationMatcher funcMatcher = functionDecl(isMain()).bind("CandidateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(nestedMatcher, &callback);
    matchFinder.addMatcher(specMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}
//source file
#include "../include/Mutator_Insert_Self_Type_Member_201.h"

// ========================================================================================================
#define MUT201_OUTPUT 1

void MutatorFrontendAction_201::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CRD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      //Filter nodes in header files
      if (!CRD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CRD->getLocation()))
        return;
      if (!CRD->isCompleteDefinition())
        return;
      if (CRD->isUnion())
        return;
      
      //Get the source code text of target node
      std::string className = CRD->getNameAsString();
      if (className.empty())
        return;
      
      //Generate unique member name
      std::string memberName = "self_member_201";
      int counter = 1;
      while (true) {
          bool nameExists = false;
          for (auto decl : CRD->decls()) {
              if (auto *FD = dyn_cast<FieldDecl>(decl)) {
                  if (FD->getNameAsString() == memberName) {
                      nameExists = true;
                      break;
                  }
              }
          }
          if (!nameExists && generatedNames.find(memberName) == generatedNames.end())
              break;
          memberName = "self_member_201_" + std::to_string(counter++);
      }
      generatedNames.insert(memberName);
      
      //Perform mutation on the source code text by applying string replacement
      std::string insertion = "\n    /*mut201*/" + className + " " + memberName + ";\n";
      
      //Find the best insertion point before the closing brace
      SourceLocation insertLoc = CRD->getBraceRange().getEnd();
      if (insertLoc.isValid()) {
          //Replace the original AST node with the mutated one
          Rewrite.InsertTextBefore(insertLoc, insertion);
      }
    }
}
  
void MutatorFrontendAction_201::MutatorASTConsumer_201::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition(), unless(isUnion())).bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
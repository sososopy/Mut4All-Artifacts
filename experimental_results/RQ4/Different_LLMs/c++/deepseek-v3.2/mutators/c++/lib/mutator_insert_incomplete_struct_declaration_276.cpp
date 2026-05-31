//source file
#include "../include/Mutator_Insert_Incomplete_Struct_Declaration_276.h"

// ========================================================================================================
#define MUT276_OUTPUT 1

void MutatorFrontendAction_276::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SD = Result.Nodes.getNodeAs<clang::RecordDecl>("StructDecl")) {
      //Filter nodes in header files
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;
      //Only target struct definitions that are complete (have a body)
      if (!SD->isCompleteDefinition() || !SD->isStruct())
        return;
      //Get the source code text of target node
      SourceRange bodyRange = SD->getSourceRange();
      SourceLocation endLoc = SD->getEndLoc();
      //Determine the struct name to use for the incomplete declaration
      std::string structName;
      if (SD->getName().empty()) {
        //Anonymous struct: look for a named struct in the surrounding context
        const DeclContext *parent = SD->getDeclContext();
        while (parent) {
          if (const RecordDecl *parentRecord = dyn_cast<RecordDecl>(parent)) {
            if (!parentRecord->getName().empty()) {
              structName = parentRecord->getNameAsString();
              break;
            }
          }
          parent = parent->getParent();
        }
        //If no named struct found, use a generic name
        if (structName.empty()) {
          structName = "MutatedStruct";
        }
      } else {
        structName = SD->getNameAsString();
      }
      //Perform mutation on the source code text by applying string replacement
      std::string insertion = "\nstruct " + structName + ";";
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextBefore(endLoc, insertion);
    }
}
  
void MutatorFrontendAction_276::MutatorASTConsumer_276::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = recordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
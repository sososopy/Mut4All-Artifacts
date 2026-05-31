//source file
#include "../include/Insert_Spurious_Identifier_Before_Constructor_354.h"

// ========================================================================================================
#define MUT354_OUTPUT 1

void MutatorFrontendAction_354::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("ConstructorDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      //Get the source code text of target node
      SourceRange range = CD->getSourceRange();
      if (!range.isValid()) return;
      std::string constructorText = stringutils::rangetoStr(*(Result.SourceManager), range);
      //(optional)Record the node information to be used in the mutation process
      //Check if identifier conflicts with existing names in the class scope
      const CXXRecordDecl* recordDecl = CD->getParent();
      if (!recordDecl) return;
      std::string chosenIdentifier;
      for (const std::string& id : identifierPool) {
        bool conflict = false;
        for (Decl* decl : recordDecl->decls()) {
          if (auto *namedDecl = dyn_cast<NamedDecl>(decl)) {
            if (namedDecl->getDeclName().isIdentifier() && namedDecl->getNameAsString() == id) {
              conflict = true;
              break;
            }
          }
        }
        if (!conflict) {
          chosenIdentifier = id;
          break;
        }
      }
      if (chosenIdentifier.empty()) chosenIdentifier = identifierPool[0];
      //Perform mutation on the source code text by applying string replacement
      size_t constructorNamePos = constructorText.find(CD->getNameAsString());
      if (constructorNamePos != std::string::npos) {
        constructorText.insert(constructorNamePos, chosenIdentifier + " ");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(range), constructorText);
    }
}
  
void MutatorFrontendAction_354::MutatorASTConsumer_354::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl().bind("ConstructorDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
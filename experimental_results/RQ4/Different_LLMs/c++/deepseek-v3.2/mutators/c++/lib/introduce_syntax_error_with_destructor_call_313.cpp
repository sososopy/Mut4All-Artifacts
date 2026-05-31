//source file
#include "../include/Mutator_313.h"

// ========================================================================================================
#define MUT313_OUTPUT 1

void MutatorFrontendAction_313::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;

      // First, check if there's a class with destructor in the TU
      ASTContext &Ctx = *Result.Context;
      bool hasDestructorClass = false;
      std::string existingClassName;
      for (const auto *Decl : Ctx.getTranslationUnitDecl()->decls()) {
        if (const auto *RD = dyn_cast<CXXRecordDecl>(Decl)) {
          if (RD->hasUserDeclaredDestructor() && RD->isCompleteDefinition()) {
            hasDestructorClass = true;
            existingClassName = RD->getNameAsString();
            break;
          }
        }
      }

      // If no such class exists, insert one before the function
      if (!hasDestructorClass) {
        generatedClassName = "Mut313_D";
        std::string classDef = "\nclass " + generatedClassName + " { public: ~" + generatedClassName + "(); };";
        SourceLocation insertLoc = FD->getBeginLoc();
        Rewrite.InsertTextBefore(insertLoc, classDef);
        classInserted = true;
      } else {
        generatedClassName = existingClassName;
        classInserted = false;
      }

      // Now mutate the function body: remove semicolon from a statement and append destructor call
      const Stmt *Body = FD->getBody();
      if (!Body) return;

      // Find a suitable statement inside the body (e.g., a DeclStmt)
      for (const auto *Child : Body->children()) {
        if (const auto *DS = dyn_cast<DeclStmt>(Child)) {
          SourceRange range = DS->getSourceRange();
          std::string origText = stringutils::rangetoStr(*Result.SourceManager, range);
          // Remove trailing semicolon if present
          if (origText.back() == ';') {
            origText.pop_back();
          }
          // Append destructor call without proper separation
          std::string mutatedText = origText + " " + generatedClassName + "::~" + generatedClassName + "();";
          mutatedText = "/*mut313*/" + mutatedText;
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(range, mutatedText);
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_313::MutatorASTConsumer_313::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
//source file
#include "../include/uninitialized_member_access_335.h"

// ========================================================================================================
#define MUT335_OUTPUT 1

void MutatorFrontendAction_335::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Ctor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Ctor")) {
      //Filter nodes in header files
      if (!Ctor || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Ctor->getLocation()))
        return;

      // Find the class declaration
      if (const auto *ClassDecl = dyn_cast<CXXRecordDecl>(Ctor->getParent())) {
        for (const auto *Field : ClassDecl->fields()) {
          if (!Field->hasInClassInitializer()) {
            TargetField = Field;
            break;
          }
        }
      }

      if (TargetField) {
        //Get the source code text of target node
        auto ConstructorText = stringutils::rangetoStr(*(Result.SourceManager),
                                                       Ctor->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string CommentText = "/* " + TargetField->getType().getAsString() + " " + TargetField->getNameAsString() + "; */\n";
        std::string InitText = TargetField->getNameAsString() + "(0)";

        // Comment out the field declaration and attempt to initialize it
        ConstructorText.insert(ConstructorText.find("{") + 1, InitText + ", ");
        ConstructorText = CommentText + ConstructorText;

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Ctor->getSourceRange()), ConstructorText);
      }
    }
}
  
void MutatorFrontendAction_335::MutatorASTConsumer_335::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl().bind("Ctor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
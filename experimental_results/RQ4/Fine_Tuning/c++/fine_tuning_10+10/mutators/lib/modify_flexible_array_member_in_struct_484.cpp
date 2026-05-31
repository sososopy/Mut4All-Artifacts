//source file
#include "../include/modify_flexible_array_member_in_struct_484.h"

// ========================================================================================================
#define MUT484_OUTPUT 1

void MutatorFrontendAction_484::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SD = Result.Nodes.getNodeAs<clang::RecordDecl>("StructDecl")) {
      //Filter nodes in header files
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;

      // Ensure the struct has a flexible array member
      bool hasFlexibleArray = false;
      for (auto *field : SD->fields()) {
        if (field->getType()->isIncompleteArrayType()) {
          hasFlexibleArray = true;
          break;
        }
      }

      if (hasFlexibleArray) {
        //Get the source code text of target node
        auto structText = stringutils::rangetoStr(*(Result.SourceManager), SD->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string bufferDecl = "char buffer[10];\n";
        auto insertionPoint = structText.find("char data[];");
        if (insertionPoint != std::string::npos) {
          structText.insert(insertionPoint, bufferDecl);
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(SD->getSourceRange()), structText);
      }
    }
}
  
void MutatorFrontendAction_484::MutatorASTConsumer_484::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = recordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
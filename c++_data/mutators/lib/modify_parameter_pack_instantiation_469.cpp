//source file
#include "../include/modify_parameter_pack_instantiation_469.h"

// ========================================================================================================
#define MUT469_OUTPUT 1

void MutatorFrontendAction_469::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTSD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!CTSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTSD->getLocation()))
        return;

      //Get the source code text of target node
      const clang::TemplateArgumentList &templateArgs = CTSD->getTemplateArgs();
      std::string newInstantiation;
      bool modified = false;

      for (unsigned i = 0; i < templateArgs.size(); ++i) {
        if (templateArgs[i].getKind() == clang::TemplateArgument::Type) {
          if (i == templateArgs.size() - 1) {
            newInstantiation += "Wrapper<" + templateArgs[i].getAsType().getAsString() + ">";
            modified = true;
          } else {
            newInstantiation += templateArgs[i].getAsType().getAsString();
          }
        }
        if (i < templateArgs.size() - 1) {
          newInstantiation += ", ";
        }
      }

      if (modified) {
        auto range = CTSD->getSourceRange();
        std::string original = stringutils::rangetoStr(*(Result.SourceManager), range);
        size_t start = original.find('<');
        size_t end = original.rfind('>');
        if (start != std::string::npos && end != std::string::npos) {
          original.replace(start + 1, end - start - 1, newInstantiation);
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(range), original);
        }
      }
    }
}
  
void MutatorFrontendAction_469::MutatorASTConsumer_469::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
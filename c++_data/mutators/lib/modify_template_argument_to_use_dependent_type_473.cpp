//source file
#include "../include/Mutator_473.h"

// ========================================================================================================
#define MUT473_OUTPUT 1

void MutatorFrontendAction_473::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Alias")) {
      if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getLocation()))
        return;

      if (const auto *TST = TA->getUnderlyingType()->getAs<TemplateSpecializationType>()) {
        if (TST->getNumArgs() != 1)
          return;

        const TemplateArgument &Arg = TST->getArg(0);
        if (Arg.getKind() != TemplateArgument::Type)
          return;

        const Type *ArgType = Arg.getAsType().getTypePtr();
        if (!ArgType || !ArgType->isDependentType())
          return;

        std::string OldAlias = stringutils::rangetoStr(*(Result.SourceManager),
                                                       TA->getSourceRange());

        std::string NewAlias = "MyStruct<Wrapper<T>::x>";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TA->getSourceRange()), NewAlias);
      }
    }
}
  
void MutatorFrontendAction_473::MutatorASTConsumer_473::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasDecl(hasType(templateSpecializationType(hasAnyTemplateArgument(templateArgument().bind("Alias"))))).bind("Alias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
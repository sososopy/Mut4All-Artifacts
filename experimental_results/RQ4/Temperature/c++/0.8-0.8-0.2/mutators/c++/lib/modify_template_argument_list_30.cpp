//source file
#include "../include/Mutator_modify_template_argument_list_30.h"

// ========================================================================================================
#define MUT30_OUTPUT 1

void MutatorFrontendAction_30::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AliasDecl = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("typeAlias")) {
      if (!AliasDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AliasDecl->getLocation()))
        return;
      
      if (const auto *TypeAlias = AliasDecl->getUnderlyingType()->getAsTagDecl()) {
        if (const auto *ClassTemplateSpecialization = dyn_cast<ClassTemplateSpecializationDecl>(TypeAlias)) {
          if (ClassTemplateSpecialization->getSpecializedTemplate()->getNameAsString() == "variant") {
            auto Location = ClassTemplateSpecialization->getLocation();
            auto Range = ClassTemplateSpecialization->getSourceRange();
            auto OriginalCode = stringutils::rangetoStr(*(Result.SourceManager), SourceRange(Range));

            size_t lastComma = OriginalCode.rfind(',');
            if (lastComma != std::string::npos) {
              OriginalCode.insert(lastComma + 1, "...");
              Rewrite.ReplaceText(SourceRange(Range), OriginalCode);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_30::MutatorASTConsumer_30::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = typeAliasDecl(hasType(asString("std::__1::variant"))).bind("typeAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
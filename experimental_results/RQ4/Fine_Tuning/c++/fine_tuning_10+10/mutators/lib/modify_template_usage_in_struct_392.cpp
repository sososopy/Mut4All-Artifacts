//source file
#include "../include/modify_template_usage_in_struct_392.h"

// ========================================================================================================
#define MUT392_OUTPUT 1

void MutatorFrontendAction_392::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("StructDecl")) {
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
        return;

      if (SD->isCompleteDefinition()) {
        std::string structContent = stringutils::rangetoStr(*(Result.SourceManager), SD->getSourceRange());
        const auto &templateArgs = SD->getTemplateArgs();
        if (templateArgs.size() > 0) {
          // Assuming the first template parameter is an integer
          auto param = templateArgs[0].getAsIntegral().getLimitedValue();
          std::string newMember = "ASPtrTy<" + std::to_string(param + 1) + "> OffsetBase; // New member using a modified template argument\n";
          structContent.insert(structContent.rfind('}'), "/*mut392*/" + newMember);
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(SD->getSourceRange()), structContent);
        }
      }
    }
}
  
void MutatorFrontendAction_392::MutatorASTConsumer_392::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
//source file
#include "../include/Specialize_Member_Function_Template_146.h"

// ========================================================================================================
#define MUT146_OUTPUT 1

void MutatorFrontendAction_146::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("MemberFunctionTemplate")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      for (auto *Specialization : MT->specializations()) {
        auto specializationText = stringutils::rangetoStr(*(Result.SourceManager),
                                                          Specialization->getSourceRange());
        std::string newSpecializationText = specializationText;
        
        // Modify the specialization argument
        size_t pos = newSpecializationText.find('<');
        if (pos != std::string::npos) {
          newSpecializationText.replace(pos + 1, 1, "3"); // Change 2 to 3
        }
        
        // Add additional specialization
        newSpecializationText += "\n/*mut146*/template<> auto " + MT->getNameAsString() + "<-1>() { return [](auto y) { return y * 2; }; }";

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Specialization->getSourceRange()), newSpecializationText);
      }
    }
}
  
void MutatorFrontendAction_146::MutatorASTConsumer_146::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasParent(classTemplateDecl())).bind("MemberFunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
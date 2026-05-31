//source file
#include "../include/Replace_Trailing_Return_Type_In_Template_Struct_423.h"

// ========================================================================================================
#define MUT423_OUTPUT 1

void MutatorFrontendAction_423::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateStruct")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';
      auto methods = MT->methods();
      for (auto method : methods) {
        auto method_name = method->getNameAsString();
        if (method_name == "operator()") {
          auto ret_type =
              stringutils::rangetoStr(*(Result.SourceManager),
                                      method->getReturnTypeSourceRange());
          llvm::outs() << ret_type << '\n';
          if (ret_type.find("decltype") != string::npos) {
            auto pos = ret_type.rfind(')');
            if (pos != string::npos) {
              ret_type.insert(pos, ",decltype(l)");
            }
            llvm::outs() << ret_type << '\n';
            content.replace(content.find("auto operator()"), ret_type.length(),
                            ret_type);
          }
        }
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_423::MutatorASTConsumer_423::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct(), isTemplateInstantiation(), has(cxxBaseSpecifier(hasType(cxxRecordDecl(isLambda()))))).bind("TemplateStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
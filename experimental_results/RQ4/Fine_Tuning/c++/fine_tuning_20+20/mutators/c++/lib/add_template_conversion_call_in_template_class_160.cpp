//source file
#include "../include/add_template_conversion_call_in_template_class_160.h"

// ========================================================================================================
#define MUT160_OUTPUT 1

void MutatorFrontendAction_160::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      if (CL->isTemplated() == false)
        return;
      cur_classes.push_back(CL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isTemplated() == false)
        return;
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Using) {
        }
      }
      for (std::size_t j = 0; j < cur_classes.size(); ++j) {
        if (DL->isDerivedFrom(cur_classes[j])) {
          auto methods = DL->methods();
          std::vector<CXXConversionDecl *> convs;
          for (auto method : methods) {
            if (auto conv = dyn_cast<CXXConversionDecl>(method)) {
              if (conv->isTemplated())
                convs.push_back(conv);
            }
          }
          if (convs.empty())
            return;
          auto target = convs[getrandom::getRandomIndex(convs.size() - 1)];
          auto target_name = target->getNameAsString();
          auto DL_name = DL->getNameAsString();
          string DL_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                       DL->getSourceRange());
          if (DL_content.find('{') == string::npos)
            return;
          DL_content.insert(DL_content.find('{') + 1,
                            "/*mut160*/\n" + DL_name + "::" + target_name +
                                "<int>();\n");
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(DL->getSourceRange()), DL_content);
        }
      }
    }
}
  
void MutatorFrontendAction_160::MutatorASTConsumer_160::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher =
        cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}
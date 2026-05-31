//source file
#include "../include/specialize_base_inherited_constructor_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    cur_classes.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    auto decls = DL->decls();
    for (auto decl : decls) {
      if (decl->getIdentifierNamespace() ==
          Decl::IdentifierNamespace::IDNS_Using) {
      }
    }
    if (DL->isCompleteDefinition() == false)
      return;
    for (std::size_t j = 0; j < cur_classes.size(); ++j) {
      if (DL->isDerivedFrom(cur_classes[j])) {
        auto ctors = cur_classes[j]->ctors();
        for (auto ctor : ctors) {
          if (ctor->getAccess() == AccessSpecifier::AS_public) {
            auto ctorname = ctor->getNameAsString();
            auto params = ctor->parameters();
            std::string param_str = "";
            for (auto param : params) {
              param_str +=
                  stringutils::rangetoStr(*(Result.SourceManager),
                                          param->getSourceRange()) +
                  ",";
            }
            if (param_str.length() > 0)
              param_str.pop_back();
            llvm::outs() << param_str << '\n';
            auto derivedname = DL->getNameAsString();
            auto ins = derivedname + "(" + param_str + ")=" +
                       cur_classes[j]->getNameAsString() + "<" + param_str +
                       ">" + "::" + ctorname + ";\n";
            ins = "/*mut4*/" + ins;
            Rewrite.ReplaceText(DL->getEndLoc(), 0, ins);
          }
        }
      }
    }
  }
}

void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}
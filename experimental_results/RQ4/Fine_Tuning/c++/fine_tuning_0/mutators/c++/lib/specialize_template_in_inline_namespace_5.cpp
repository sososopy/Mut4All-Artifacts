//source file
#include "../include/Specialize_Template_In_Inline_Namespace_5.h"

// ========================================================================================================
#define MUT5_OUTPUT 1

void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("InlineNS")) {
    if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                   NS->getLocation()))
      return;
    if (!NS->isInlineNamespace())
      return;
    inlined_namespaces.push_back(NS);
    llvm::outs() << NS->getNameAsString() << '\n';
  } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>(
                 "InlineTemplate")) {
    if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FT->getLocation()))
      return;
    inline_templates.push_back(FT);
    llvm::outs() << FT->getNameAsString() << '\n';
  } else if (auto *FD =
                 Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunc")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    if (!FD->isTemplateInstantiation())
      return;
    auto name = FD->getNameAsString();
    llvm::outs() << name << '\n';
    auto args = FD->getTemplateSpecializationArgs();
    std::string spe = "template<>";
    for (auto arg : args->asArray()) {
      if (arg.getKind() == clang::TemplateArgument::Type) {
        spe += " ";
        spe += arg.getAsType().getAsString();
      } else if (arg.getKind() == clang::TemplateArgument::Integral) {
        spe += " ";
        llvm::SmallVector<char, 10> str;
        arg.getAsIntegral().toString(str, 10);
        spe += std::string(str.begin(), str.end());
      }
    }
    spe += " ";
    spe += name;
    spe += "(";
    for (auto param : FD->parameters()) {
      spe += param->getType().getAsString();
      spe += ",";
    }
    spe.back() = ')';
    spe += ";\n";
    spe = "/*mut5*/" + spe;
    Rewrite.ReplaceText(FD->getBeginLoc(), 0, spe);
  }
}
  
void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto inlineNS_matcher = namespaceDecl().bind("InlineNS");
  auto inlineTemplate_matcher = functionTemplateDecl().bind("InlineTemplate");
  auto templateFunc_matcher = functionDecl().bind("TemplateFunc");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(inlineNS_matcher, &callback);
  matchFinder.addMatcher(inlineTemplate_matcher, &callback);
  matchFinder.addMatcher(templateFunc_matcher, &callback);
  matchFinder.matchAST(Context);
  for (auto ns : callback.inlined_namespaces) {
    auto decls = ns->decls();
    for (auto decl : decls) {
      if (decl->getKind() == clang::Decl::FunctionTemplate) {
        auto func_template =
            llvm::dyn_cast<clang::FunctionTemplateDecl>(decl);
        auto insts = func_template->specializations();
        for (auto inst : insts) {
          llvm::outs() << inst->getNameAsString() << '\n';
        }
      }
    }
  }
}
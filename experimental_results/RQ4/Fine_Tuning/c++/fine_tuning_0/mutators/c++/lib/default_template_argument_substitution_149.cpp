//source file
#include "../include/Default_Template_Argument_Substitution_149.h"

// ========================================================================================================
#define MUT149_OUTPUT 1

void MutatorFrontendAction_149::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAlias")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find('=') != string::npos) {
        cur_alias.push_back(DL);
      }
    }
    //Check whether the matched AST node is the target node
    else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isCompleteDefinition())
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("template") == string::npos)
        return;
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Using) {
          cur_alias.push_back(static_cast<const TypeAliasDecl *>(decl));
        }
      }
    }
    //Check whether the matched AST node is the target node
    else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isTemplateInstantiation())
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("template") == string::npos)
        return;
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Using) {
          cur_alias.push_back(static_cast<const TypeAliasDecl *>(decl));
        }
      }
    }
    //Check whether the matched AST node is the target node
    else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("Main")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->getNameAsString() != "main")
        return;
      //Get the source code text of target node
      for (std::size_t i = 0; i < cur_alias.size(); ++i) {
        auto target = cur_alias[i];
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               target->getSourceRange());
        if (content.find("template") != string::npos) {
          auto name = target->getNameAsString();
          auto ins = "/*mut149*/" + name + "<>";
          Rewrite.ReplaceText(DL->getEndLoc(), 0, ins);
        }
      }
    }
  }

void MutatorFrontendAction_149::MutatorASTConsumer_149::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    auto matcher = typeAliasDecl().bind("TypeAlias");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto func_matcher = functionDecl().bind("Functions");
    auto main_matcher = functionDecl().bind("Main");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.addMatcher(main_matcher, &callback);
    matchFinder.matchAST(Context);
}
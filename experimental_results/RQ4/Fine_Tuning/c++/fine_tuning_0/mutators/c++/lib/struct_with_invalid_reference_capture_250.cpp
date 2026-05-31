//source file
#include "../include/Struct_With_Invalid_Reference_Capture_250.h"

// ========================================================================================================
#define MUT250_OUTPUT 1

void MutatorFrontendAction_250::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isStruct())
        return;
      auto methods = DL->methods();
      for (auto method : methods) {
        if (method->isConstexpr()) {
          auto params = method->parameters();
          for (auto param : params) {
            cur_vars.push_back(param);
          }
        }
      }
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isMain())
        return;
      auto params = DL->parameters();
      for (auto param : params) {
        cur_vars.push_back(param);
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("Var")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isLocalVarDeclOrParm() == false)
        return;
      if (DL->isExceptionVariable() || DL->isNRVOVariable())
        return;
      cur_vars.push_back(DL);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Main")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isMain())
        return;
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Tag) {
          auto tagdecl = dyn_cast<TagDecl>(decl);
          if (tagdecl->isStruct()) {
            auto struct_decl = dyn_cast<CXXRecordDecl>(tagdecl);
            auto methods = struct_decl->methods();
            for (auto method : methods) {
              if (method->isConstexpr()) {
                auto params = method->parameters();
                for (auto param : params) {
                  cur_vars.push_back(param);
                }
              }
            }
          }
        }
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "StructInMain")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isStruct())
        return;
      auto methods = DL->methods();
      for (auto method : methods) {
        if (method->isConstexpr()) {
          auto params = method->parameters();
          for (auto param : params) {
            cur_vars.push_back(param);
          }
        }
      }
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::VarDecl>("VarInMain")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isLocalVarDeclOrParm() == false)
        return;
      if (DL->isExceptionVariable() || DL->isNRVOVariable())
        return;
      cur_vars.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "StructInMain2")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isStruct())
        return;
      if (cur_vars.empty())
        return;
      auto var = cur_vars[getrandom::getRandomIndex(cur_vars.size() - 1)];
      auto var_name = var->getNameAsString();
      string ins = "return " + var_name;
      ins = "/*mut250*/" + ins;
      auto methods = DL->methods();
      for (auto method : methods) {
        if (method->isConstexpr()) {
          Rewrite.ReplaceText(method->getBody()->getEndLoc(), 0, ins);
        }
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "StructInMain3")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isStruct())
        return;
      if (cur_vars.empty())
        return;
      auto var = cur_vars[getrandom::getRandomIndex(cur_vars.size() - 1)];
      auto var_name = var->getNameAsString();
      string ins = "return " + var_name;
      ins = "/*mut250*/" + ins;
      auto methods = DL->methods();
      for (auto method : methods) {
        if (method->isConstexpr()) {
          Rewrite.ReplaceText(method->getBody()->getEndLoc(), 0, ins);
        }
      }
    }
  }

void MutatorFrontendAction_250::MutatorASTConsumer_250::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Struct");
    auto function_matcher = functionDecl().bind("Function");
    auto var_matcher = varDecl().bind("Var");
    auto main_matcher = functionDecl().bind("Main");
    auto struct_in_main_matcher =
        cxxRecordDecl(hasAncestor(functionDecl(isMain()))).bind("StructInMain");
    auto var_in_main_matcher =
        varDecl(hasAncestor(functionDecl(isMain()))).bind("VarInMain");
    auto struct_in_main_matcher2 =
        cxxRecordDecl(hasAncestor(functionDecl(isMain()))).bind(
            "StructInMain2");
    auto struct_in_main_matcher3 =
        cxxRecordDecl(hasAncestor(functionDecl(isMain()))).bind(
            "StructInMain3");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.addMatcher(main_matcher, &callback);
    matchFinder.addMatcher(struct_in_main_matcher, &callback);
    matchFinder.addMatcher(var_in_main_matcher, &callback);
    matchFinder.addMatcher(struct_in_main_matcher2, &callback);
    matchFinder.addMatcher(struct_in_main_matcher3, &callback);
    matchFinder.matchAST(Context);
}
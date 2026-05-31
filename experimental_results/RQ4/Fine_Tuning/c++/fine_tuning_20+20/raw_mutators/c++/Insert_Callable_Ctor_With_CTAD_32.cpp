//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Callable_Ctor_With_CTAD_32
 */ 
class MutatorFrontendAction_32 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(32)

private:
    class MutatorASTConsumer_32 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_32(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        std::vector<const clang::CXXRecordDecl *> cur_classes;
        std::vector<string> cur_callables;
        std::vector<string> cur_callables_type;
    };
};

//source file
#include "../include/insert_callable_ctor_with_ctad_32.h"

// ========================================================================================================
#define MUT32_OUTPUT 1

void MutatorFrontendAction_32::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if (!CL->isCompleteDefinition())
        return;
      if (CL->isLambda())
        return;
      if (CL->isStruct() == false && CL->isClass() == false)
        return;
      auto name = CL->getNameAsString();
      if (name == "")
        return;
      auto decls = CL->decls();
      bool has_ctor = false;
      for (auto decl : decls) {
        if (isa<CXXConstructorDecl>(decl)) {
          has_ctor = true;
          break;
        }
      }
      if (has_ctor == false)
        return;
      cur_classes.push_back(CL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Callables")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isCompleteDefinition())
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      auto name = DL->getNameAsString();
      if (name == "")
        return;
      auto decls = DL->decls();
      bool has_op = false;
      for (auto decl : decls) {
        if (isa<CXXMethodDecl>(decl)) {
          auto method = cast<CXXMethodDecl>(decl);
          if (method->getNameAsString() == "operator()") {
            has_op = true;
            break;
          }
        }
      }
      if (has_op == false)
        return;
      cur_callables.push_back(name);
      cur_callables_type.push_back("class");
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("Lambdas")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isLocalVarDeclOrParm() == false)
        return;
      auto type = DL->getType();
      if (type->isStructureType() == false)
        return;
      auto name = DL->getNameAsString();
      if (name == "")
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find('[') == string::npos)
        return;
      if (content.find(']') == string::npos)
        return;
      if (content.find('(') == string::npos)
        return;
      if (content.find(')') == string::npos)
        return;
      if (content.find('{') == string::npos)
        return;
      if (content.find('}') == string::npos)
        return;
      cur_callables.push_back(name);
      cur_callables_type.push_back("lambda");
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isMain() == false)
        return;
      if (cur_callables.empty())
        return;
      if (cur_classes.empty())
        return;
      auto index = getrandom::getRandomIndex(cur_classes.size() - 1);
      auto callable_index = getrandom::getRandomIndex(cur_callables.size() - 1);
      auto callable_name = cur_callables[callable_index];
      auto callable_type = cur_callables_type[callable_index];
      auto target_class = cur_classes[index];
      auto target_name = target_class->getNameAsString();
      auto decls = target_class->decls();
      bool has_target_ctor = false;
      for (auto decl : decls) {
        if (isa<CXXConstructorDecl>(decl)) {
          auto ctor = cast<CXXConstructorDecl>(decl);
          if (ctor->isTemplated() == false)
            continue;
          auto params = ctor->parameters();
          if (params.size() != 1)
            continue;
          auto param_type = params[0]->getType();
          if (param_type->isDependentType() == false)
            continue;
          has_target_ctor = true;
          break;
        }
      }
      if (has_target_ctor == false)
        return;
      string ins = "";
      if (callable_type == "class") {
        ins = "auto mut_32 = " + target_name + "(" + callable_name + "());";
      } else if (callable_type == "lambda") {
        ins = "auto mut_32 = " + target_name + "(" + callable_name + ");";
      } else
        return;
      ins = "/*mut32*/" + ins;
      Rewrite.ReplaceText(FD->getEndLoc(), 0, ins);
    }
  }

void MutatorFrontendAction_32::MutatorASTConsumer_32::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto callable_matcher = cxxRecordDecl().bind("Callables");
    auto lambda_matcher = varDecl().bind("Lambdas");
    auto function_matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(callable_matcher, &callback);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.matchAST(Context);
}
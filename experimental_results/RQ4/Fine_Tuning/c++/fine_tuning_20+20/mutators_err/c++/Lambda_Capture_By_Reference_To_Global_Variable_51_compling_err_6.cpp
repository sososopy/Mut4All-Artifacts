//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_Capture_By_Reference_To_Global_Variable_51
 */ 
class MutatorFrontendAction_51 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(51)

private:
    class MutatorASTConsumer_51 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_51(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        const clang::VarDecl *global_var = nullptr;
        const clang::CXXRecordDecl *global_type = nullptr;
        std::string global_name = "";
    };
};

//source file
#include "../include/lambda_capture_by_reference_to_global_variable_51.h"

// ========================================================================================================
#define MUT51_OUTPUT 1

void MutatorFrontendAction_51::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("GlobalVar")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->isLocalVarDeclOrParm() || VD->isStaticLocal())
        return;
      auto type = VD->getType();
      if (!type->isBuiltinType() && !type->isEnumeralType() &&
          !type->isRecordType())
        return;
      if (type->isBuiltinType()) {
        if (type.getAsString() != "int")
          return;
      }
      global_var = VD;
      global_name = VD->getNameAsString();
      llvm::outs() << "global var: " << global_name << '\n';
    } else if (auto *TD =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("GlobalType")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      if ((!TD->isStruct() && !TD->isClass()) || TD->isLambda())
        return;
      if (!TD->isCompleteDefinition())
        return;
      global_type = TD;
      global_name = TD->getNameAsString();
      llvm::outs() << "global type: " << global_name << '\n';
    } else if (auto *LD = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!LD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LD->getBeginLoc()))
        return;
      if (global_name == "")
        return;
      if (LD->hasExplicitParameters() == false)
        return;
      auto lambda_type = LD->getLambdaClass()->getLambdaTypeInfo();
      auto lambda_type_str =
          stringutils::rangetoStr(*(Result.SourceManager),
                                  lambda_type->getTypeLoc().getSourceRange());
      if (lambda_type_str.find("void") != string::npos)
        return;
      llvm::outs() << lambda_type_str << '\n';
      auto captures = LD->captures();
      if (std::distance(captures.begin(), captures.end()) > 0)
        return;
      auto body = LD->getBody();
      auto returns = body->getDescendants<ReturnStmt>();
      if (std::distance(returns.begin(), returns.end()) > 0)
        return;
      auto lambda_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                LD->getSourceRange());
      if (lambda_str.find('[') == string::npos)
        return;
      if (lambda_str.find(']') == string::npos)
        return;
      if (lambda_str.find("[]") != string::npos) {
        lambda_str.replace(lambda_str.find("[]"), 2, "[&" + global_name + "]");
      } else if (lambda_str.find("[=") != string::npos) {
        lambda_str.insert(lambda_str.find("[=") + 2, "&" + global_name + ",");
      } else if (lambda_str.find("[&") != string::npos) {
        lambda_str.insert(lambda_str.find("[&") + 2, global_name + ",");
      }
      if (global_var != nullptr) {
        lambda_str = stringutils::replaceReturnType(lambda_str,
                                                    global_var->getType());
      } else if (global_type != nullptr) {
        lambda_str = stringutils::replaceReturnType(lambda_str,
                                                    global_type->getTypeForDecl());
      }
      lambda_str = stringutils::replaceReturnValue(lambda_str, global_name);
      llvm::outs() << "mut51: " << lambda_str << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LD->getSourceRange()),
                          lambda_str);
    } else if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Class")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      auto DLs = CL->decls();
      for (auto DL : DLs) {
        if (DL->getKind() == Decl::Kind::CXXMethod) {
          auto MD = dyn_cast<CXXMethodDecl>(DL);
          if (MD->isStatic())
            continue;
          if (MD->hasBody() == false)
            continue;
          if (MD->isConstexpr())
            continue;
          if (MD->isImplicit())
            continue;
          if (MD->isVirtual())
            continue;
          auto body = MD->getBody();
          auto lambdas = body->getDescendants<LambdaExpr>();
          if (std::distance(lambdas.begin(), lambdas.end()) == 0)
            continue;
          auto lambda = lambdas[0];
          if (lambda->hasExplicitParameters() == false)
            continue;
          auto lambda_type = lambda->getLambdaClass()->getLambdaTypeInfo();
          auto lambda_type_str = stringutils::rangetoStr(
              *(Result.SourceManager),
              lambda_type->getTypeLoc().getSourceRange());
          if (lambda_type_str.find("void") != string::npos)
            continue;
          auto captures = lambda->captures();
          if (std::distance(captures.begin(), captures.end()) > 0)
            continue;
          auto lambda_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                    lambda->getSourceRange());
          if (lambda_str.find('[') == string::npos)
            continue;
          if (lambda_str.find(']') == string::npos)
            continue;
          if (lambda_str.find("[]") != string::npos) {
            lambda_str.replace(lambda_str.find("[]"), 2,
                               "[&" + global_name + ",this]");
          } else if (lambda_str.find("[=") != string::npos) {
            lambda_str.insert(lambda_str.find("[=") + 2,
                              "&" + global_name + ",this,");
          } else if (lambda_str.find("[&") != string::npos) {
            lambda_str.insert(lambda_str.find("[&") + 2,
                              global_name + ",this,");
          }
          if (global_var != nullptr) {
            lambda_str = stringutils::replaceReturnType(lambda_str,
                                                        global_var->getType());
          } else if (global_type != nullptr) {
            lambda_str = stringutils::replaceReturnType(lambda_str,
                                                        global_type->getTypeForDecl());
          }
          lambda_str = stringutils::replaceReturnValue(lambda_str, global_name);
          llvm::outs() << "mut51: " << lambda_str << '\n';
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(lambda->getSourceRange()),
              lambda_str);
        }
      }
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Func")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isConstexpr())
        return;
      if (FD->isImplicit())
        return;
      auto body = FD->getBody();
      auto lambdas = body->getDescendants<LambdaExpr>();
      if (std::distance(lambdas.begin(), lambdas.end()) == 0)
        return;
      auto lambda = lambdas[0];
      if (lambda->hasExplicitParameters() == false)
        return;
      auto lambda_type = lambda->getLambdaClass()->getLambdaTypeInfo();
      auto lambda_type_str =
          stringutils::rangetoStr(*(Result.SourceManager),
                                  lambda_type->getTypeLoc().getSourceRange());
      if (lambda_type_str.find("void") != string::npos)
        return;
      auto captures = lambda->captures();
      if (std::distance(captures.begin(), captures.end()) > 0)
        return;
      auto lambda_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                lambda->getSourceRange());
      if (lambda_str.find('[') == string::npos)
        return;
      if (lambda_str.find(']') == string::npos)
        return;
      if (lambda_str.find("[]") != string::npos) {
        lambda_str.replace(lambda_str.find("[]"), 2, "[&" + global_name + "]");
      } else if (lambda_str.find("[=") != string::npos) {
        lambda_str.insert(lambda_str.find("[=") + 2, "&" + global_name + ",");
      } else if (lambda_str.find("[&") != string::npos) {
        lambda_str.insert(lambda_str.find("[&") + 2, global_name + ",");
      }
      if (global_var != nullptr) {
        lambda_str = stringutils::replaceReturnType(lambda_str,
                                                    global_var->getType());
      } else if (global_type != nullptr) {
        lambda_str = stringutils::replaceReturnType(lambda_str,
                                                    global_type->getTypeForDecl());
      }
      lambda_str = stringutils::replaceReturnValue(lambda_str, global_name);
      llvm::outs() << "mut51: " << lambda_str << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambda->getSourceRange()),
                          lambda_str);
    }
}
  
void MutatorFrontendAction_51::MutatorASTConsumer_51::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto globalvar_matcher = varDecl().bind("GlobalVar");
    auto globaltype_matcher = cxxRecordDecl().bind("GlobalType");
    auto lambda_matcher = lambdaExpr().bind("Lambda");
    auto class_matcher = cxxRecordDecl().bind("Class");
    auto func_matcher = functionDecl().bind("Func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(globalvar_matcher, &callback);
    matchFinder.addMatcher(globaltype_matcher, &callback);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}
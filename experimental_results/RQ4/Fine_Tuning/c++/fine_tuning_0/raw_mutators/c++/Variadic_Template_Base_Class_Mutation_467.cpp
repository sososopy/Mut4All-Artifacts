//header file
#pragma once
#include "Mutator_base.h"

/**
 * Variadic_Template_Base_Class_Mutation_467
 */ 
class MutatorFrontendAction_467 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(467)

private:
    class MutatorASTConsumer_467 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_467(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/Variadic_Template_Base_Class_Mutation_467.h"

// ========================================================================================================
#define MUT467_OUTPUT 1

void MutatorFrontendAction_467::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    if (DL->isStruct() == false)
      return;
    if (DL->isLambda())
      return;
    cur_classes.push_back(DL);
  } else if (auto *DL =
                 Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    auto templatedecl = DL->getTemplatedDecl();
    auto bases = templatedecl->bases();
    auto params = DL->getTemplateParameters();
    if (params->size() != 1)
      return;
    auto param = params->getParam(0);
    auto param_name = param->getName();
    if (param->isParameterPack() == false)
      return;
    if (bases.size() != 1)
      return;
    auto base = *(bases.begin());
    auto base_type = base.getType();
    if (base_type.getAsString().find(param_name) == string::npos)
      return;
    llvm::outs() << "mut467: find target template\n";
    auto decls = templatedecl->decls();
    for (auto decl : decls) {
      if (decl->getIdentifierNamespace() ==
          Decl::IdentifierNamespace::IDNS_Using) {
        return;
      }
    }
    string ins = "using " + param_name.str() + " :: k ...;";
    ins = "/*mut467*/" + ins;
    llvm::outs() << ins << '\n';
    Rewrite.ReplaceText(templatedecl->getEndLoc(), 0, ins);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("Vars")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    auto type = DL->getType();
    if (type.getAsString().find("X") == string::npos)
      return;
    auto name = DL->getName();
    string ins = name.str() + ".A::k=1;";
    ins = "/*mut467*/" + ins;
    llvm::outs() << ins << '\n';
    Rewrite.ReplaceText(DL->getEndLoc(), 0, ins);
  } else if (auto *DL =
                 Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    auto function_name = DL->getName();
    if (function_name != "t")
      return;
    llvm::outs() << "mut467: find target function\n";
    auto decls = DL->decls();
    for (auto decl : decls) {
      if (decl->getIdentifierNamespace() ==
          Decl::IdentifierNamespace::IDNS_Using) {
        return;
      }
    }
    string ins = "X<A, B> x;";
    ins = "/*mut467*/" + ins;
    llvm::outs() << ins << '\n';
    Rewrite.ReplaceText(DL->getBody()->getBeginLoc(), 0, ins);
  } else if (auto *DL =
                 Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
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
        auto fields = cur_classes[j]->fields();
        std::vector<FieldDecl *> public_field;
        std::vector<FieldDecl *> protected_field;
        for (auto field : fields) {
          if (field->getAccess() == AccessSpecifier::AS_protected)
            protected_field.push_back(field);
          else if (field->getAccess() == AccessSpecifier::AS_public)
            public_field.push_back(field);
        }
        if (!protected_field.empty()) {
          string acc_sep = "\npublic:\n";
          string classname = cur_classes[j]->getNameAsString();
          for (auto field : protected_field) {
            string fieldname = field->getNameAsString();
            acc_sep += "using " + classname + " :: " + fieldname + ";\n";
          }
          llvm::outs() << acc_sep;
          Rewrite.ReplaceText(DL->getEndLoc(), 0, acc_sep);
        }
        if (!public_field.empty()) {
          string acc_sep = "protected:\n";
          string classname = cur_classes[j]->getNameAsString();
          for (auto field : public_field) {
            string fieldname = field->getNameAsString();
            acc_sep += "using " + classname + " :: " + fieldname + ";\n";
          }
          acc_sep = "\n/*mut3*/" + acc_sep;
          Rewrite.ReplaceText(DL->getEndLoc(), 0, acc_sep);
        }
      }
    }
  }
}

void MutatorFrontendAction_467::MutatorASTConsumer_467::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
  auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
  auto template_matcher = classTemplateDecl().bind("Templates");
  auto var_matcher = varDecl().bind("Vars");
  auto function_matcher = functionDecl().bind("Functions");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(derived_matcher, &callback);
  matchFinder.addMatcher(template_matcher, &callback);
  matchFinder.addMatcher(var_matcher, &callback);
  matchFinder.addMatcher(function_matcher, &callback);
  matchFinder.matchAST(Context);
}
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Inheritance_From_Derived_Class_486
 */ 
class MutatorFrontendAction_486 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(486)

private:
    class MutatorASTConsumer_486 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_486(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> derived_classes;
        std::vector<const clang::CXXRecordDecl *> all_classes;
    };
};

//source file
#include "../include/Remove_Inheritance_From_Derived_Class_486.h"

// ========================================================================================================
#define MUT486_OUTPUT 1

void MutatorFrontendAction_486::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      derived_classes.push_back(DL);
    } else if (auto *CL =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if (CL->isCompleteDefinition() == false)
        return;
      all_classes.push_back(CL);
    }
    return;
  }

void MutatorFrontendAction_486::MutatorASTConsumer_486::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
    for (auto DL : callback.derived_classes) {
      auto bases = DL->bases();
      for (auto base : bases) {
        auto base_type = base.getType();
        auto base_decl = base_type->getAsCXXRecordDecl();
        if (base_decl == nullptr)
          continue;
        llvm::outs() << base_decl->getNameAsString() << '\n';
      }
      auto DL_name = DL->getNameAsString();
      auto DL_content = stringutils::rangetoStr(*(Context.getSourceManager()),
                                                DL->getSourceRange());
      llvm::outs() << DL_content << '\n';
      auto DL_bases = DL->bases();
      std::vector<string> base_names;
      for (auto base : DL_bases) {
        auto base_type = base.getType();
        auto base_decl = base_type->getAsCXXRecordDecl();
        if (base_decl == nullptr)
          continue;
        base_names.push_back(base_decl->getNameAsString());
      }
      llvm::outs() << DL_name << '\n';
      llvm::outs() << DL_content << '\n';
      DL_content = stringutils::eraseBase(DL_content);
      llvm::outs() << DL_content << '\n';
      auto DL_decls = DL->decls();
      for (auto decl : DL_decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Using) {
          llvm::outs() << "using decl\n";
          DL_content = stringutils::eraseUsing(DL_content);
        }
      }
      llvm::outs() << DL_content << '\n';
      DL_content = stringutils::eraseOverride(DL_content);
      llvm::outs() << DL_content << '\n';
      Rewrite.ReplaceText(DL->getSourceRange(), DL_content);
      auto DL_fields = DL->fields();
      for (auto field : DL_fields) {
        auto field_type = field->getType();
        auto field_type_decl = field_type->getAsCXXRecordDecl();
        if (field_type_decl == nullptr)
          continue;
        auto field_type_name = field_type_decl->getNameAsString();
        for (auto base_name : base_names) {
          if (field_type_name == base_name) {
            llvm::outs() << "base type used\n";
            Rewrite.ReplaceText(field->getSourceRange(), "mut486");
          }
        }
      }
      auto DL_methods = DL->methods();
      for (auto method : DL_methods) {
        auto method_name = method->getNameAsString();
        llvm::outs() << method_name << '\n';
        for (auto base_name : base_names) {
          if (method_name == base_name) {
            llvm::outs() << "base type used\n";
            Rewrite.ReplaceText(method->getSourceRange(), "mut486(){}");
          }
        }
      }
    }
    for (auto CL : callback.all_classes) {
      auto CL_name = CL->getNameAsString();
      llvm::outs() << CL_name << '\n';
      for (auto DL : callback.derived_classes) {
        auto DL_name = DL->getNameAsString();
        if (DL_name == CL_name) {
          llvm::outs() << "derived class\n";
          auto CL_content = stringutils::rangetoStr(*(Context.getSourceManager()),
                                                    CL->getSourceRange());
          if (CL_content.find('{') == CL_content.rfind('}')) {
            llvm::outs() << "empty class\n";
            CL_content.insert(CL_content.rfind('}'), "mut486(){}");
            Rewrite.ReplaceText(CL->getSourceRange(), CL_content);
          }
        }
      }
    }
}
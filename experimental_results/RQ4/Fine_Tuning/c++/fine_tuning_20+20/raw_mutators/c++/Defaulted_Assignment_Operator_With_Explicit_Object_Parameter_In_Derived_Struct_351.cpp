//header file
#pragma once
#include "Mutator_base.h"

/**
 * Defaulted_Assignment_Operator_With_Explicit_Object_Parameter_In_Derived_Struct_351
 */ 
class MutatorFrontendAction_351 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(351)

private:
    class MutatorASTConsumer_351 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_351(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> base_structs;
    };
};

//source file
#include "../include/defaulted_assignment_operator_with_explicit_object_parameter_in_derived_struct_351.h"

// ========================================================================================================
#define MUT351_OUTPUT 1

void MutatorFrontendAction_351::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStruct() == false || DL->isCompleteDefinition() == false)
        return;
      if (DL->isDerivedFrom("Mut351_base") == false)
        return;
      llvm::outs() << DL->getNameAsString() << '\n';
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Using) {
          llvm::outs() << "using\n";
        }
      }
      auto methods = DL->methods();
      for (auto method : methods) {
        if (method->isCopyAssignmentOperator()) {
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 method->getSourceRange());
          llvm::outs() << content << '\n';
          auto pos = content.find('(');
          if (pos != string::npos) {
            content.insert(pos + 1, "Mut351_base &");
            content += "=default;";
            content = "/*mut351*/" + content;
            Rewrite.ReplaceText(
                CharSourceRange::getTokenRange(method->getSourceRange()),
                content);
          }
        }
      }
    } else if (auto *BL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Base")) {
      if (!BL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BL->getLocation()))
        return;
      if (BL->isStruct() == false || BL->isCompleteDefinition() == false)
        return;
      base_structs.push_back(BL);
    }
}

void MutatorFrontendAction_351::MutatorASTConsumer_351::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto base_matcher = cxxRecordDecl().bind("Base");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(base_matcher, &callback);
    matchFinder.matchAST(Context);
}
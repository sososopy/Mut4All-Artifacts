//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Variadic_Template_Parameter_Pack_333
 */ 
class MutatorFrontendAction_333 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(333)

private:
    class MutatorASTConsumer_333 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_333(Rewriter &R) : TheRewriter(R) {}
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
        
    };
};

//source file
#include "../include/Add_Variadic_Template_Parameter_Pack_333.h"

// ========================================================================================================
#define MUT333_OUTPUT 1

void MutatorFrontendAction_333::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("VariadicTemplate")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      llvm::outs() << declaration << '\n';
      if (MT->getKind() == Decl::Kind::FunctionTemplate) {
        auto func = MT->getAsFunction();
        if (!func->isDefined())
          return;
        auto func_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                    func->getSourceRange());
        llvm::outs() << func_content << '\n';
        if (func_content.find("return") != string::npos) {
          auto pos = func_content.find("return");
          func_content.insert(pos, "auto a=tuple{};/*mut333*/");
        } else {
          auto pos = func_content.rfind('}');
          func_content.insert(pos, "auto a=tuple{};return a;/*mut333*/");
        }
        llvm::outs() << func_content << '\n';
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(func->getSourceRange()), func_content);
      } else if (MT->getKind() == Decl::Kind::ClassTemplate) {
        auto class_ = dyn_cast<ClassTemplateDecl>(MT);
        if (!class_->isThisDeclarationADefinition())
          return;
        auto class_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                     class_->getSourceRange());
        llvm::outs() << class_content << '\n';
        if (class_content.find("return") != string::npos) {
          auto pos = class_content.find("return");
          class_content.insert(pos, "auto a=tuple{};/*mut333*/");
        } else {
          auto pos = class_content.rfind('}');
          class_content.insert(pos, "auto a=tuple{};return a;/*mut333*/");
        }
        llvm::outs() << class_content << '\n';
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(class_->getSourceRange()), class_content);
      }
      auto pos = declaration.find("...");
      declaration.insert(pos + 3, " T... Args");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_333::MutatorASTConsumer_333::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = templateDecl(has(templateTypeParmDecl(hasName("T")))).bind("VariadicTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
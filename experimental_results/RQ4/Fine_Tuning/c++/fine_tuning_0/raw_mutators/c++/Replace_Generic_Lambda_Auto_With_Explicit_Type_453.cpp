//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Generic_Lambda_Auto_With_Explicit_Type_453
 */ 
class MutatorFrontendAction_453 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(453)

private:
    class MutatorASTConsumer_453 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_453(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> existingTypes;
    };
};

//source file
#include "../include/Replace_Generic_Lambda_Auto_With_Explicit_Type_453.h"

// ========================================================================================================
#define MUT453_OUTPUT 1

void MutatorFrontendAction_453::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TL = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunc")) {
      if (!TL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TL->getLocation()))
        return;
      if (!TL->isThisDeclarationADefinition())
        return;
      llvm::outs() << TL->getNameAsString() << '\n';
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), TL->getSourceRange());
      auto lambda_matcher = cxxRecordDecl(isLambda()).bind("lambda");
      auto lambdas = match(lambda_matcher, *TL, *(Result.Context));
      if (lambdas.empty())
        return;
      auto DL = lambdas[0].getNodeAs<CXXRecordDecl>("lambda");
      assert(DL != nullptr);
      llvm::outs() << DL->getNameAsString() << '\n';
      auto lambda_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                    DL->getSourceRange());
      llvm::outs() << lambda_content << '\n';
      auto auto_matcher = varDecl(hasType(autoType())).bind("auto");
      auto autos = match(auto_matcher, *DL, *(Result.Context));
      if (autos.empty())
        return;
      auto VD = autos[0].getNodeAs<VarDecl>("auto");
      assert(VD != nullptr);
      llvm::outs() << VD->getNameAsString() << '\n';
      auto auto_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                  VD->getSourceRange());
      llvm::outs() << auto_content << '\n';
      auto auto_pos = lambda_content.find(auto_content);
      assert(auto_pos != string::npos);
      auto type_matcher = namedDecl().bind("type");
      auto types = match(type_matcher, *TL->getTranslationUnitDecl(),
                         *(Result.Context));
      for (auto type : types) {
        auto TD = type.getNodeAs<NamedDecl>("type");
        assert(TD != nullptr);
        existingTypes.push_back(TD->getNameAsString());
      }
      auto type_name = getrandom::getRandomTypeName(existingTypes);
      llvm::outs() << type_name << '\n';
      lambda_content.replace(auto_pos, auto_content.length(), type_name);
      llvm::outs() << lambda_content << '\n';
      content.replace(content.find(lambda_content), lambda_content.length(),
                      lambda_content);
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(TL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_453::MutatorASTConsumer_453::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("TemplateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
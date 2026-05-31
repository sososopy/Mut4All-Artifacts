//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Vector_Element_Type_198
 */ 
class MutatorFrontendAction_198 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(198)

private:
    class MutatorASTConsumer_198 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_198(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TypeAliasDecl *> alias_decls;
        std::vector<const clang::TypeAliasTemplateDecl *> alias_template_decls;
        std::vector<const clang::TypedefDecl *> typedef_decls;
        std::vector<const clang::VarDecl *> vector_decls;
    };
};

//source file
#include "../include/modify_vector_element_type_198.h"

// ========================================================================================================
#define MUT198_OUTPUT 1

void MutatorFrontendAction_198::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AL = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Alias")) {
      if (!AL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AL->getLocation()))
        return;
      alias_decls.push_back(AL);
    } else if (auto *ALT = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>(
                   "AliasTemplate")) {
      if (!ALT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      ALT->getLocation()))
        return;
      alias_template_decls.push_back(ALT);
    } else if (auto *TD =
                   Result.Nodes.getNodeAs<clang::TypedefDecl>("Typedef")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      typedef_decls.push_back(TD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vectors")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    }
}

void MutatorFrontendAction_198::MutatorASTConsumer_198::HandleTranslationUnit(
    ASTContext &Context) {
  MatchFinder Finder;
  Callback callback(TheRewriter);
  Finder.addMatcher(typeAliasDecl().bind("Alias"), &callback);
  Finder.addMatcher(typeAliasTemplateDecl().bind("AliasTemplate"), &callback);
  Finder.addMatcher(typedefDecl().bind("Typedef"), &callback);
  Finder.addMatcher(varDecl().bind("Vectors"), &callback);
  Finder.addMatcher(varDecl(hasType(hasUnqualifiedDesugaredType(
                            recordType(hasDeclaration(cxxRecordDecl(
                                hasName("::std::vector")))))))
                        .bind("Vector"),
                    &callback);
  Finder.matchAST(Context);
}

std::string get_vector_element_type(const clang::VarDecl *VD) {
  auto type = VD->getType();
  if (type->isVectorType() == false)
    return "";
  if (type->isInstantiationDependentType())
    return "";
  auto record_type = type->getAs<clang::RecordType>();
  if (!record_type)
    return "";
  auto record_decl = record_type->getDecl();
  if (!record_decl)
    return "";
  if (record_decl->getNameAsString() != "vector")
    return "";
  auto cxx_record_decl = llvm::dyn_cast<clang::CXXRecordDecl>(record_decl);
  if (!cxx_record_decl)
    return "";
  auto template_decl = cxx_record_decl->getDescribedClassTemplate();
  if (!template_decl)
    return "";
  auto specialization_decl =
      llvm::dyn_cast<clang::ClassTemplateSpecializationDecl>(cxx_record_decl);
  if (!specialization_decl)
    return "";
  auto args = specialization_decl->getTemplateArgs().asArray();
  if (args.size() == 0)
    return "";
  auto arg = args[0];
  if (arg.getKind() != clang::TemplateArgument::Type)
    return "";
  return arg.getAsType().getAsString();
}

void MutatorFrontendAction_198::EndSourceFileAction() {
  if (MUT198_OUTPUT) {
    llvm::errs() << "MUT198: ";
    for (auto VD : vector_decls) {
      llvm::errs() << VD->getNameAsString() << " ";
    }
    llvm::errs() << "\n";
  }
  if (vector_decls.size() == 0)
    return;
  auto VD = vector_decls[0];
  auto element_type = get_vector_element_type(VD);
  if (element_type == "")
    return;
  auto loc = VD->getBeginLoc();
  TheRewriter.InsertText(loc, "using mut198_element_type = " + element_type +
                                     ";\nusing mut198_vector_type = std::vector<mut198_element_type>;\n");
  TheRewriter.ReplaceText(loc, VD->getType().getAsString().size(),
                          "mut198_vector_type");
}

std::unique_ptr<clang::ASTConsumer>
MutatorFrontendAction_198::CreateASTConsumer(clang::CompilerInstance &CI,
                                             llvm::StringRef InFile) {
  llvm::errs() << "MUT198: Start processing\n";
  TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
  return std::make_unique<MutatorASTConsumer_198>(TheRewriter);
}
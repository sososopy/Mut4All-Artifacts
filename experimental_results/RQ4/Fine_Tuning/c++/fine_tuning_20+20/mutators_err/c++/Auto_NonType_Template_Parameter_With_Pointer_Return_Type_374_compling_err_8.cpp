//header file
#pragma once
#include "Mutator_base.h"

/**
 * Auto_NonType_Template_Parameter_With_Pointer_Return_Type_374
 */ 
class MutatorFrontendAction_374 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(374)

private:
    class MutatorASTConsumer_374 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_374(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl *> function_templates;
    };
};

//source file
#include "../include/auto_nontype_template_parameter_with_pointer_return_type_374.h"

// ========================================================================================================
#define MUT374_OUTPUT 1

void MutatorFrontendAction_374::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NTTP = Result.Nodes.getNodeAs<clang::NonTypeTemplateParmDecl>("NTTP")) {
      if (!NTTP || !Result.Context->getSourceManager().isWrittenInMainFile(
                       NTTP->getLocation()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          NTTP->getSourceRange());
      llvm::outs() << "NTTP: " << text << '\n';
      stringutils::strReplace(text, "int", "auto");
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(NTTP->getSourceRange()), text);
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>(
                   "FunctionTemplate")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getLocation()))
        return;
      llvm::outs() << "FT: " << FT->getNameAsString() << '\n';
      function_templates.push_back(FT);
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isTemplateInstantiation())
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FD->getSourceRange());
      llvm::outs() << "FD: " << text << '\n';
      stringutils::strReplace(text, "int", "auto");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()),
                          text);
    } else if (auto *RT =
                   Result.Nodes.getNodeAs<clang::TemplateSpecializationType>(
                       "TemplateSpecializationType")) {
      if (!RT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          RT->getSourceRange());
      llvm::outs() << "RT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(RT->getSourceRange()), text);
      }
    } else if (auto *R = Result.Nodes.getNodeAs<clang::ReturnStmt>("Return")) {
      if (!R || !Result.Context->getSourceManager().isWrittenInMainFile(
                    R->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          R->getSourceRange());
      llvm::outs() << "R: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(R->getSourceRange()),
                            text);
      }
    } else if (auto *DT =
                   Result.Nodes.getNodeAs<clang::DecltypeType>("Decltype")) {
      if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          DT->getSourceRange());
      llvm::outs() << "DT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(DT->getSourceRange()), text);
      }
    } else if (auto *PT =
                   Result.Nodes.getNodeAs<clang::PointerType>("PointerType")) {
      if (!PT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          PT->getSourceRange());
      llvm::outs() << "PT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(PT->getSourceRange()), text);
      }
    } else if (auto *TST = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>(
                   "TemplateSpecializationType")) {
      if (!TST || !Result.Context->getSourceManager().isWrittenInMainFile(
                      TST->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          TST->getSourceRange());
      llvm::outs() << "TST: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(TST->getSourceRange()), text);
      }
    } else if (auto *TT = Result.Nodes.getNodeAs<clang::TypedefType>("Typedef")) {
      if (!TT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      TT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          TT->getSourceRange());
      llvm::outs() << "TT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(TT->getSourceRange()), text);
      }
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::TypedefDecl>("TypedefDecl")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      TD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          TD->getSourceRange());
      llvm::outs() << "TD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(TD->getSourceRange()), text);
      }
    } else if (auto *ET = Result.Nodes.getNodeAs<clang::EnumType>("EnumType")) {
      if (!ET || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ET->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          ET->getSourceRange());
      llvm::outs() << "ET: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(ET->getSourceRange()), text);
      }
    } else if (auto *ETD = Result.Nodes.getNodeAs<clang::EnumType>("EnumTypeDecl")) {
      if (!ETD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      ETD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          ETD->getSourceRange());
      llvm::outs() << "ETD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(ETD->getSourceRange()), text);
      }
    } else if (auto *ET = Result.Nodes.getNodeAs<clang::ElaboratedType>("ElaboratedType")) {
      if (!ET || !Result.Context->getSourceManager().isWrittenInMainFile(
                      ET->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          ET->getSourceRange());
      llvm::outs() << "ET: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(ET->getSourceRange()), text);
      }
    } else if (auto *ETD = Result.Nodes.getNodeAs<clang::EnumType>("ElaboratedTypeDecl")) {
      if (!ETD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      ETD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          ETD->getSourceRange());
      llvm::outs() << "ETD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(ETD->getSourceRange()), text);
      }
    } else if (auto *F = Result.Nodes.getNodeAs<clang::FieldDecl>("Field")) {
      if (!F || !Result.Context->getSourceManager().isWrittenInMainFile(
                     F->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          F->getSourceRange());
      llvm::outs() << "F: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(F->getSourceRange()), text);
      }
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FD->getSourceRange());
      llvm::outs() << "FD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), text);
      }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionTypeDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FTD->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FTD->getSourceRange());
      llvm::outs() << "FTD: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace(text, "int", "auto");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FTD->getSourceRange()), text);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionType>("FunctionType")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FT->getSourceRange());
      llvm::outs() << "FT: " << text << '\n';
      if (text.find("int") != string::npos) {
        stringutils::strReplace
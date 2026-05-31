//source file
#include "../include/Apply_Make_Integer_Seq_With_Mismatched_Args_178.h"

// ========================================================================================================
#define MUT178_OUTPUT 1

void MutatorFrontendAction_178::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *field = Result.Nodes.getNodeAs<clang::FieldDecl>("field")) {
        if (!field || !Result.Context->getSourceManager().isWrittenInMainFile(field->getLocation()))
            return;
        
        QualType fieldType = field->getType();
        if (auto *TST = dyn_cast<clang::TemplateSpecializationType>(fieldType)) {
            clang::TemplateName name = TST->getTemplateName();
            clang::TemplateDecl *TD = name.getAsTemplateDecl();
            if (auto *CTD = dyn_cast<clang::ClassTemplateDecl>(TD)) {
                auto *TemplatedDecl = CTD->getTemplatedDecl();
                if (TemplatedDecl && !TemplatedDecl->isThisDeclarationADefinition()) {
                    std::string tName = CTD->getNameAsString();
                    std::string newType = "__make_integer_seq<" + tName + ", float, 10>";
                    clang::SourceRange typeRange = field->getTypeSourceInfo()->getTypeLoc().getSourceRange();
                    if (typeRange.isValid()) {
                        Rewrite.ReplaceText(typeRange, newType);
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_178::MutatorASTConsumer_178::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl(hasParent(cxxRecordDecl())).bind("field");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
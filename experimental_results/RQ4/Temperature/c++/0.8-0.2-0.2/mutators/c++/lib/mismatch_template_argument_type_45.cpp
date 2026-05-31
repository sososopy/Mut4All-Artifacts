//source file
#include "../include/mismatch_template_argument_type_45.h"

// ========================================================================================================
#define MUT45_OUTPUT 1

void MutatorFrontendAction_45::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TS = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpec")) {
        //Filter nodes in header files
        if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Result.Context->getSourceManager().getSpellingLoc(TS->getTemplateName().getAsTemplateDecl()->getLocation())))
            return;

        if (templateInstances.find(TS) != templateInstances.end()) {
            const clang::TemplateDecl *templateDecl = templateInstances[TS];
            if (auto *classTemplate = llvm::dyn_cast<clang::ClassTemplateDecl>(templateDecl)) {
                //Get the source code text of target node
                auto templateName = classTemplate->getNameAsString();
                std::string incompatibleTypeDecl = "class IncompatibleType {}; // Added by mutation\n";
                std::string newInstance = templateName + "<IncompatibleType> instance;\n";
                //Perform mutation on the source code text by applying string replacement
                clang::SourceLocation insertLoc = Result.Context->getSourceManager().getSpellingLoc(TS->getTemplateName().getAsTemplateDecl()->getLocation()).getLocWithOffset(1);
                //Replace the original AST node with the mutated one
                Rewrite.InsertText(insertLoc, "/*mut45*/" + incompatibleTypeDecl + newInstance, true, true);
            }
        }
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
            return;
        if (auto *classTemplate = llvm::dyn_cast<clang::ClassTemplateDecl>(TD)) {
            for (auto *spec : classTemplate->specializations()) {
                if (spec->isExplicitSpecialization()) {
                    templateInstances[spec->getTypeAsWritten()->getType()->getAs<clang::TemplateSpecializationType>()] = TD;
                }
            }
        }
    }
}
  
void MutatorFrontendAction_45::MutatorASTConsumer_45::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto templateMatcher = clang::ast_matchers::decl().bind("TemplateDecl");
    auto specializationMatcher = clang::ast_matchers::templateSpecializationType().bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(specializationMatcher, &callback);
    matchFinder.matchAST(Context);
}